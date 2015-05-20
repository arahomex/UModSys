import time
import sys
import weakref
import struct
import zlib

from common import *
from l_common import *
from l_channel import *
from l_bus import *

#-------------------------------------------------------------
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
  loss_simulator = None
  #
  def __init__(self, uid):
    self.uid = uid
    #
    self.gates = []
    self.services = []
    self.buses = {}
    self.channels = {}
    #
    self.sscans = {}
    #
  #
  # -------- gates
  #
  def gate_add(self, gate):
    self.gates.append(gate)
    gate.node_setup(self)
  #
  def gate_remove(self, gate):
    self.gates.remove(gate)
    gate.node_setup(None)
  #
  # -------- services
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
  def service_on_scanresult(self, sk, level, nid, sid):
    if sk in self.sscans:
      scan = self.sscans[sk]
      scan[3](sk, level, nid, sid)
    else:
      self.d_error("Invalid service-scan %s", repr(sk))
  #
  def service_on_scan(self, ret, kwlist):
    ret.append(self.uid)
    for kw in kwlist:
      for s in self.services:
        if s.on_scan(kw):
          ret.append(s.uid)
    #
    return ret
  #
  # -------- buses
  #
  def bus_to(self, uid):
    for b in self.buses.values():
      if (b.other_uid is not None) and (uid==b.other_uid):
        return b
    return None
  #
  # -------- channels
  #
  def channel_open(self, serv, nid, sid, func, options=None):
    aux = None
    if options is None:
      mode = 0
    elif type(options) in (list, tuple):
      mode, aux = Channel.mode_decode(options)
    elif type(options) is int:
      mode = options
    elif type(options) is str:
      mode,aux = Channel.mode_decode(options.split(','))
    if mode is None:
      raise Exception('Bad channel options '+repr(options))
    return self.channel_open_u(serv, nid, sid, func, mode, aux, None)
  #
  def channel_close(self, ch):
    ch.disconnect()
    del self.channels[ch.uid]
    #ch.node_setup(None)
  #
  def channel_open_u(self, serv, nid, sid, func, mode, aux, chid2):
    self.nchannels = self.nchannels+1
    ch = Channel(serv, self.nchannels, (nid, sid, func), mode, aux, chid2)
    self.channels[ch.uid] = ch
    ch.node_setup(self)
    ch.connect()
    return ch
  #
  def channel_initate(self, addr, chid, chid2, mode, aux, ConnectFlag):
    for busid,bus in self.buses.iteritems():
      if bus.other_uid==addr[0]:
        if ConnectFlag:
          if chid2 is None:
            opts = [str(chid), addr[0], addr[1], addr[2]]
            if len(mode)!=0:
              opts.append(','.join(mode))
            if (aux is not None) and len(aux)>0:
              opts.extend(aux)
            bus.syscmd_emit('C_OPEN', opts, True)
            break
          else:
            bus.syscmd_emit('C_OPENED', (str(chid2), str(chid)), True)
            break
        else:
          bus.syscmd_emit('C_CLOSE', (str(chid2), str(chid)), True)
          break
  #
  def channel_on_open(self, syscmd, argv):
    mode,_ = Channel.mode_decode(argv[4].split(','))
    if mode is None:
      self.d_warning("Bad channel mode %s", repr(argv[4]))
      syscmd.nak('BAD_CHANNEL_MODE')
      return True
    if argv[0] in self.channels:
      self.d_warning("Bad channel %s", repr(argv[0]))
      syscmd.nak('BAD_CHANNEL')
      return True
    if argv[1]!=self.uid:
      self.d_warning("Bad node %s", repr(argv[1]))
      syscmd.nak('BAD_NODE')
      return True
    aux = argv[4:]
    for s in self.services:
      if s.uid==argv[2]:
        if s.on_channel(argv[2], argv[3], mode, aux):
          syscmd.ack()
          ch = self.channel_open_u(s, syscmd.bus.other_uid, None, None, mode, aux, argv[0])
          ch.bus = syscmd.bus
          s.on_connect(ch)
        else:
          syscmd.nak('BAD_SERVICE_FUNC')
        return True
    syscmd.nak('BAD_SERVICE')
    return True
  #
  def channel_on_opened(self, syscmd, argv):
    if argv[0] not in self.channels:
      syscmd.nak('BAD_CHANNEL')
      return True
    syscmd.ack()
    #
    ch = self.channels[argv[0]]
    ch.other_uid = argv[1]
    ch.bus = syscmd.bus
    ch.service.on_connect(ch)
    return True
  #
  def channel_on_close(self, syscmd, argv):
    if argv[0] not in channels:
      syscmd.nak('BAD_CHANNEL')
      return True
    syscmd.ack()
    #
    ch = self.channels[argv[0]]
    ch.service.on_disconnect(ch)
  #
  # -------- ON handlers
  #
  def on_tick(self, tick):
    for gate in self.gates:
      gate.on_tick(tick)
    #
    for busid in self.buses.keys():
      self.buses[busid].on_tick(tick)
    #
    for serv in self.services:
      serv.on_tick(tick)
    #
    for chid in self.channels.keys():
      self.channels[chid].on_tick(tick)
    #
    for sk in self.sscans.keys():
      scan = self.sscans[sk]
      scan[4] = scan[4] - tick
      if scan[4]<0:
        self.service_scan_stop(sk)
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
    if self.loss_simulator is not None:
      if self.loss_simulator[0](self.loss_simulator, self, gate, addr, frame):
        self.d_debug("on_gate_frame LOSS %s %s", repr(addr), repr(frame))
        return # drop a packet
    #
    bus = self.buses[(gate, addr)]
    self.d_debug("on_gate_frame %s %s", bus.uid, repr(frame))
    if frame[1] == 0:
      bus.on_sysframe(frame)
    elif frame[1] in self.channels:
      self.channels[frame[1]].on_frame(frame[0], frame[2], frame[3], frame[4])
    else:
      self.d_warning("bad channel %d", frame[1])
    pass
  #
  pass


#-------------------------------------------------------------
#-------------------------------------------------------------

