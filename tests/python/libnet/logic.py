import time
import sys
import weakref
import struct
import zlib

from com import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class NodeObject(object):
  #
  uid = None
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
      dbg(2, "%s::node_setup node=%s" % (self.uid, repr(self.node)))
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

#-------------------------------------------------------------


class Node(object):
  class Line(NodeObject):
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
        self.node().syscmd[command](self, int(key), chkey, command, args)
      pass
  #
  uid = None
  #
  gates = None
  services = None
  lines = None
  #
  sscans = None
  #
  syscmd = None
  #
  def __init__(self, uid):
    self.uid = uid
    self.gates = []
    self.services = []
    self.lines = {}
    #
    self.sscans = {}
    #
    self.syscmd = {}
    self.syscmd['G_OPEN'] = self.on_syscmd_gate_open
    self.syscmd['S_SCAN'] = self.on_syscmd_service_scan
    self.syscmd['S_SCAN_RESULT'] = self.on_syscmd_service_scanresult
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
    for lineid,line in self.lines.iteritems():
      line.syscmd_emit('S_SCAN', (scan[1], scan[0], scan[2]), True)
    return sk
  #
  def service_scan_stop(self, sk):
    if sk in self.sscans:
      del self.sscans[sk]
      dbg(3, "service_scan_stop %s" % sk)
  #
  def service_scan_result(self, sk, level, nid, sid):
    if sk in self.sscans:
      scan = self.sscans[sk]
      scan[3](sk, level, nid, sid)
  #
  #
  def tick(self, tick):
    for gate in self.gates:
      gate.tick(tick)
    for sk in self.sscans.keys():
      scan = self.sscans[sk]
      scan[4] = scan[4] - tick
      if scan[4]<0:
        self.service_scan_stop(sk)
    #
  #
  #
  def on_gate_connected(self, gate, addr, aux):
    line = self.Line(gate, addr, aux)
    self.lines[(gate, addr)] = line
    line.node_setup(self)
    #
    dbg(3, "on_gate_connected %s" % line.uid)
    line.syscmd_emit('G_OPEN', self.uid, True)
    pass
  #
  def on_gate_disconnected(self, gate, addr, aux=None):
    line = self.lines[(gate, addr)]
    del self.lines[(gate, addr)]
    line.node_setup(None)
    #
    dbg(3, "on_gate_disconnected %s" % line.uid)
    pass
  #
  def on_gate_frame(self, gate, addr, frame):
    line = self.lines[(gate, addr)]
    dbg(4, "on_gate_frame %s %s" % (line.uid, repr(frame)))
    if frame[1] == 0:
      line.frame_syscmd(frame[0], frame[2], frame[3], frame[4])
    pass
  #
  #
  def on_syscmd_gate_open(self, line, sid, chkey, command, args):
    uid, args = next_word(args)
    if uid=='':
      line.syscmd_emit_nak(sid, 'BAD_COMMAND')
    else:
      line.other_uid = uid
      line.syscmd_emit_ack(sid)
  #
  def on_syscmd_service_scan(self, line, sid, chkey, command, args):
    level, args = next_word(args)
    if level=='':
      line.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    try:
      level = int(level)
    except:
      line.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    sk, args = next_word(args)
    if sk=='':
      line.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    line.syscmd_emit_ack(sid)
    #
    ret = [str(level), sk, self.uid]
    while True:
      kw, args = next_word(args)
      if kw=='':
        break
      for s in self.services:
        if s.on_scan(kw):
          ret.append(s.uid)
    line.syscmd_emit('S_SCAN_RESULT', ' '.join(ret), True)
    #
  def on_syscmd_service_scanresult(self, line, sid, chkey, command, args):
    level, args = next_word(args)
    if level=='':
      line.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    sk, args = next_word(args)
    if sk=='':
      line.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    nid, args = next_word(args)
    if nid=='':
      line.syscmd_emit_nak(sid, 'BAD_COMMAND')
      return
    while True:
      sid, args = next_word(args)
      if sid=='':
        break
      self.service_scan_result(sk, level, nid, sid)
    #
  #
  pass


#-------------------------------------------------------------
#-------------------------------------------------------------

