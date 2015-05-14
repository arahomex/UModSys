import time
import sys
import weakref
import struct
import zlib

from com import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Gate(object):
  #
  uid = None
  node = None
  #
  pass


#-------------------------------------------------------------

class Node(object):
  class Line(object):
    #
    sid = None
    xaddr = None
    queue = None
    #
    gate = None
    addr = None
    aux = None
    #
    def __init__(self, gate, addr, aux):
      self.gate = gate
      self.addr = addr
      self.aux = aux
      #
      self.xaddr = "%s::%s" % (gate.uid, addr)
      self.sid = 0
      self.queue = {}
    #
    def next_sid(self, callback=None):
      self.sid = sid = self.sid+1
      return sid
    #
    def make_syscmd(self, command, args=None, callback=None, sid=None):
      # <sid> :: <command> <args>
      isys = sid is not None
      if not isys:
        sid = self.next_sid()
      if args is not None:
        if is_array(args):
          args = str.join(args, ' ')
        command = "%s %s" % (command, args)
      frame = make_frame(isys, 0, '', "%d" % sid, command)
      self.queue[sid] = (sid, callback, frame)
      return frame
    #
    def done_syscmd(self, sid, success, extra=''):
      if sid in self.queue:
        cmd = self.queue[sid]
        del self.queue[sid]
        if cmd[1] is not None:
          cmd[1](cmd[0], cmd[2], success, extra)
    #
    def on_syscmd(self, node, key, sid, command, args):
      if command=='G_OPEN':
        frame = self.make_syscmd('ACK', '', None, sid)
      else:
        frame = self.make_syscmd('NAK', 'BAD_COMMAND', None, sid)
      self.frame_emit(frame)
    #
    def frame_emit(self, frame):
      self.gate.frame_emit(self.addr, self.aux, frame)
    #
    def frame_syscmd(self, node, sysflag, chkey, key, value):
      command, args = value.split(' ', 1)
      if sysflag:
        if command=='ACK':
          self.done_syscmd(int(key), True, args)
        elif command=='NAK':
          self.done_syscmd(int(key), False, args)
        else:
          raise Exception('Bad sys command')
      else:
        self.on_syscmd(node, chkey, int(key), command, args)
      pass
    #
  #
  uid = None
  gates = None
  lines = None
  #
  def __init__(self, uid):
    self.uid = uid
    self.gates = []
    self.lines = {}
  #
  def gate_add(self, gate):
    self.gates.append(gate)
    gate.node = self
  #
  def tick(self, tick):
    for gate in self.gates:
      gate.tick(tick)
  #
  def oncmdack_gopen(self, cmdframe, success, extra):
    pass
  #
  def on_gate_connected(self, gate, addr, aux=None):
    xaddr = "%s::%s" % (gate.uid, addr)
    line = self.Line(gate, addr, aux)
    self.lines[(gate, addr)] = line
    dbg(3, "on_gate_connected %s" % line.xaddr)
    frame = line.make_syscmd('G_OPEN', (self.uid), self.oncmdack_gopen)
    line.frame_emit(frame)
    pass
  #
  def on_gate_disconnected(self, gate, addr, aux=None):
    line = self.lines[(gate, addr)]
    del self.lines[(gate, addr)]
    dbg(3, "on_gate_disconnected %s" % line.xaddr)
    pass
  #
  def on_gate_frame(self, gate, addr, frame):
    line = self.lines[(gate, addr)]
    dbg(4, "on_gate_frame %s %s" % (line.xaddr, repr(frame)))
    if frame[1] == 0:
      line.frame_syscmd(self, frame[0], frame[2], frame[3], frame[4])
    pass
  #


#-------------------------------------------------------------
#-------------------------------------------------------------

