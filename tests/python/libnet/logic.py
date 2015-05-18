import time
import sys
import weakref
import struct
import zlib

from com import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class NodeObject(BaseObject):
  #
  node = None
  #
  def __init__(self, uid):
    self.uid = uid
    #
  #
  def on_node_none(self):
    self.node_setup(self, None)
  #
  def node_setup(self, node):
    if node is None:
      self.node = None
    else:
      self.node = weakref.ref(node, self.on_node_none)
      self.d_debug("node_setup node=%s", repr(self.node))
  #
  def tick(self, tick):
    pass
  #
  pass

#-------------------------------------------------------------

class Gate(NodeObject):
  #
  def __init__(self, uid, node):
    NodeObject.__init__(self, uid)
    if node is not None:
      node.gate_add(self)
  #
  pass

#-------------------------------------------------------------

class Service(NodeObject):
  #
  def __init__(self, uid, node):
    NodeObject.__init__(self, uid)
    if node is not None:
      node.service_add(self)
  #
  def on_scan(self, kw):
    return False # passive one
  #
  def on_send(self, ch):
    pass
  #
  def on_receive(self, ch, key, value):
    pass
  #
  def on_connect(self, ch):
    pass
  #
  def on_disconnect(self, ch, errcode):
    pass
  #
  def on_channel(self, sid, func, options):
    return False # passive one
  #
  pass

#-------------------------------------------------------------

class Channel(NodeObject):
  #
  service = None
  addr = None # (nid, sid, func)
  options = None
  other_uid = None
  bus = None
  #
  def __init__(self, serv, chid, addr, options, chid2):
    NodeObject.__init__(self, chid)
    #
    self.other_uid = chid2
    self.service = serv
    self.addr = addr
    self.options = options
  #
  def connect(self):
    self.node().channel_initate(self.addr, self.uid, self.other_uid, self.options, True)
  #
  def disconnect(self):
    if self.other_uid is not None:
      self.node().channel_initate(self.addr, self.uid, self.other_uid, self.options, False)
  #
  def tick(self, tick):
    if self.other_uid is not None:
      self.d_debug("Tick request send")
      self.service.on_send(self)
  #
  def send_pair(self, key, value):
    frame = make_frame(False, self.other_uid, '', key, value)
    self.bus.frame_emit(frame)
  #
  def on_frame(self, sysflag, chkey, key, value):
    self.service.on_receive(self, key, value)
  #

#-------------------------------------------------------------

class Bus(NodeObject):
  #
  sid = None
  queue = None
  #
  gate = None
  addr = None
  aux = None
  #
  other_uid = None
  #
  def __init__(self, gate, addr, aux):
    NodeObject.__init__(self, None)
    #
    self.gate = gate
    self.addr = addr
    self.aux = aux
    #
    self.uid = "%s::%s" % (gate.uid, addr)
    self.sid = 0
    self.queue = {}
  #
  def next_sid(self, callback=None):
    self.sid = sid = self.sid+1
    return sid
  #
  def syscmd_make(self, command, args=None, callback=None, sid=None):
    # <sid> :: <command> <args>
    isys = sid is not None
    if not isys:
      sid = self.next_sid()
    if args is not None:
      if is_array(args):
        args = ' '.join(args)
      command = "%s %s" % (command, args)
    frame = make_frame(isys, 0, '', "%d" % sid, command)
    if callback is not None:
      self.queue[sid] = (sid, callback, frame)
    return frame
  #
  def syscmd_done(self, sid, success, extra=''):
    if sid in self.queue:
      cmd = self.queue[sid]
      del self.queue[sid]
      if type(cmd[1]) is bool:
        pass
      else:
        cmd[1](cmd[0], cmd[2], success, extra)
  #
  def frame_emit(self, frame):
    self.gate.frame_emit(self.addr, self.aux, frame)
  #
  def syscmd_emit(self, command, args=None, callback=None):
    frame = self.syscmd_make(command, args, callback, None)
    self.frame_emit(frame)
  #
  def syscmd_emit_ack(self, sid, ext=None):
    frame = self.syscmd_make('ACK', ext, None, sid)
    self.frame_emit(frame)
  #
  def syscmd_emit_nak(self, sid, ext=None):
    frame = self.syscmd_make('NAK', ext, None, sid)
    self.frame_emit(frame)
  #
  def frame_syscmd(self, sysflag, chkey, key, value):
    command, args = next_word(value)
    if sysflag:
      if command=='ACK':
        self.syscmd_done(int(key), True, args)
      elif command=='NAK':
        self.syscmd_done(int(key), False, args)
      else:
        #raise Exception('Bad sys command')
        pass # ignore the command
    else:
      self.node().d_debug("syscmd{%s %s %s %s}", key, repr(chkey), repr(command), repr(args))
      self.node().syscmd[command](self, int(key), chkey, command, args)
    pass

#-------------------------------------------------------------

class Node(BaseObject):
  #
  gates = None
  services = None
  buses = None
  #
  sscans = None
  channels = None
  nchannels = 0
  #
  syscmd = None
  #
  def __init__(self, uid):
    self.uid = uid
    self.gates = []
    self.services = []
    self.buses = {}
    #
    self.sscans = {}
    self.channels = {}
    #
    self.syscmd = {}
    self.syscmd['G_OPEN'] = self.on_syscmd_gate_open
    self.syscmd['S_SCAN'] = self.on_syscmd_service_scan
    self.syscmd['S_SCAN_RESULT'] = self.on_syscmd_service_scanresult
    self.syscmd['C_OPEN'] = self.on_syscmd_channel_open
    self.syscmd['C_OPENED'] = self.on_syscmd_channel_opened
    self.syscmd['C_CLOSE'] = self.on_syscmd_channel_close
  #
  #
  def gate_add(self, gate):
    self.gates.append(gate)
    gate.node_setup(self)
  #
  def gate_remove(self, gate):
    self.gates.remove(gate)
    gate.node_setup(None)
  #
  #
  def service_add(self, s):
    self.services.append(s)
    s.node_setup(self)
  #
  def service_remove(self, s):
    self.services.remove(gate)
    s.node_setup(None)
  #
  def service_scan(self, level, kws, callback, timeout):
    sk = random_key(8)
    scan = [sk, str(level), ' '.join(kws), callback, timeout]
    self.sscans[sk] = scan
    for busid,bus in self.buses.iteritems():
      bus.syscmd_emit('S_SCAN', (scan[1], scan[0], scan[2]), True)
    return sk
  #
  def service_scan_stop(self, sk):
    if sk in self.sscans:
      del self.sscans[sk]
      self.d_debug("service_scan_stop %s", sk)
  #
  def service_scan_result(self, sk, level, nid, sid):
    if sk in self.sscans:
      scan = self.sscans[sk]
      scan[3](sk, level, nid, sid)
    else:
      self.d_error("Invalid service-scan %s", repr(sk))
  #
  #
  def channel_open(self, serv, nid, sid, func, options=None, chid2=None):
    self.nchannels = self.nchannels+1
    ch = Channel(serv, self.nchannels, (nid, sid, func), options, chid2)
    self.channels[ch.uid] = ch
    ch.node_setup(self)
    ch.connect()
    return ch
  #
  def channel_close(self, ch):
    ch.disconnect()
    del self.channels[ch.uid]
    #ch.node_setup(None)
  #
  def channel_initate(self, addr, chid, chid2, options, ConnectFlag):
    for busid,bus in self.buses.iteritems():
      if bus.other_uid==addr[0]:
        if ConnectFlag:
          if chid2 is None:
            bus.syscmd_emit('C_OPEN', (str(chid), addr[0], addr[1], addr[2]), True)
            break
          else:
            bus.syscmd_emit('C_OPENED', (str(chid2), str(chid)), True)
            break
        else:
          bus.syscmd_emit('C_CLOSE', (str(chid2), str(chid)), True)
          break
  #
  def tick(self, tick):
    for gate in self.gates:
      gate.tick(tick)
    #
    for serv in self.services:
      serv.tick(tick)
    #
    for chid in self.channels.keys():
      self.channels[chid].tick(tick)
    #
    for sk in self.sscans.keys():
      scan = self.sscans[sk]
      scan[4] = scan[4] - tick
      if scan[4]<0:
        self.service_scan_stop(sk)
    #
  #
  #
  def on_gate_connected(self, gate, addr, aux):
    bus = Bus(gate, addr, aux)
    self.buses[(gate, addr)] = bus
    bus.node_setup(self)
    #
    self.d_debug("on_gate_connected %s", bus.uid)
    bus.syscmd_emit('G_OPEN', self.uid, True)
    pass
  #
  def on_gate_disconnected(self, gate, addr, aux=None):
    bus = self.buses[(gate, addr)]
    del self.buses[(gate, addr)]
    bus.node_setup(None)
    #
    self.d_debug("on_gate_disconnected %s", bus.uid)
    #raise_for_num(self, 2)
    pass
  #
  def on_gate_frame(self, gate, addr, frame):
    bus = self.buses[(gate, addr)]
    self.d_debug("on_gate_frame %s %s", bus.uid, repr(frame))
    if frame[1] == 0:
      bus.frame_syscmd(frame[0], frame[2], frame[3], frame[4])
    elif frame[1] in self.channels:
      self.channels[frame[1]].on_frame(frame[0], frame[2], frame[3], frame[4])
    else:
      self.d_warning("bad channel %d", frame[1])
    pass
  #
  #
  def on_syscmd_gate_open(self, bus, sid, chkey, command, args):
    argv, args = next_words(args, 1)
    if argv is None:
      bus.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    bus.syscmd_emit_ack(sid)
    bus.other_uid = argv[0]
  #
  def on_syscmd_service_scan(self, bus, sid, chkey, command, args):
    argv, args = next_words(args, 2, 100)
    if argv is None:
      bus.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    bus.syscmd_emit_ack(sid)
    #
    ret = [argv[0], argv[1], self.uid]
    for kw in argv[2:]:
      for s in self.services:
        if s.on_scan(kw):
          ret.append(s.uid)
    bus.syscmd_emit('S_SCAN_RESULT', ' '.join(ret), True)
  #
  def on_syscmd_service_scanresult(self, bus, sid, chkey, command, args):
    argv, args = next_words(args, 3, 100)
    if argv is None:
      bus.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    bus.syscmd_emit_ack(sid)
    #
    for sid in argv[3:]:
      self.service_scan_result(argv[1], argv[0], argv[2], sid)
  #
  def on_syscmd_channel_open(self, bus, sid, chkey, command, args):
    argv, args = next_words(args, 4, 100, (int, None, None, None))
    if argv is None:
      bus.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    if argv[0] in self.channels:
      bus.syscmd_emit_nak(sid, 'BAD_CHANNEL')
      return
    if argv[1]!=self.uid:
      bus.syscmd_emit_nak(sid, 'BAD_NODE')
      return
    for s in self.services:
      if s.uid==argv[2]:
        if s.on_channel(argv[2], argv[3], argv[3:]):
          bus.syscmd_emit_ack(sid)
          ch = self.channel_open(s, bus.other_uid, None, None, argv[3:], argv[0])
          ch.bus = bus
          s.on_connect(ch)
        else:
          bus.syscmd_emit_nak(sid, 'BAD_SERVICE_FUNC')
        return
    bus.syscmd_emit_nak(sid, 'BAD_SERVICE')
  #
  def on_syscmd_channel_opened(self, bus, sid, chkey, command, args):
    argv, args = next_words(args, 2, 100, (int,int))
    if argv is None:
      bus.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    if argv[0] not in self.channels:
      bus.syscmd_emit_nak(sid, 'BAD_CHANNEL')
      return
    bus.syscmd_emit_ack(sid)
    #
    ch = self.channels[argv[0]]
    ch.other_uid = argv[1]
    ch.bus = bus
    ch.service.on_connect(ch)
  #
  def on_syscmd_channel_close(self, bus, sid, chkey, command, args):
    argv, args = next_words(args, 2, 100, (int,))
    if argv is None:
      bus.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    if argv[0] not in channels:
      bus.syscmd_emit_nak(sid, 'BAD_CHANNEL')
      return
    bus.syscmd_emit_ack(sid)
    #
    ch = self.channels[argv[0]]
    ch.service.on_disconnect(ch)
  #
  #
  pass


#-------------------------------------------------------------
#-------------------------------------------------------------

