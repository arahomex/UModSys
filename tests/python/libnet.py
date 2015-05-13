#!/usr/bin/python -B

import time
import sys
import socket
import select
import errno
import asyncore

READ_ONLY = select.POLLIN | select.POLLPRI | select.POLLHUP | select.POLLERR
READ_WRITE = READ_ONLY | select.POLLOUT


DebugLevel = 8

def dbg(L, S):
  if L<DebugLevel:
    print >>sys.stderr, S

#-------------------------------------------------------------
#-------------------------------------------------------------

class Node(object):
  uid = None
  gates = None
  #
  def __init__(self, uid):
    self.uid = uid
    self.gates = []
  #
  def gate_add(self, gate):
    self.gates.append(gate)
    gate.node = self
  #
  def tick(self, tick):
    for gate in self.gates:
      gate.tick(tick)
  #
  def on_message(self, addr, channel, payload):
    pass

#-------------------------------------------------------------

class Gate(object):
  #
  node = None
  #
  pass

class Gate_TCP(Gate):
  class Client(asyncore.dispatcher_with_send):
    gate = None
    uid = None
    buffer = None
    child = False
    #
    def __init__(self, isChild, args):
      if isChild:
        sock, addr, gate = args
        asyncore.dispatcher_with_send.__init__(self, sock)
        self.isChild = True
        self.gate = gate
        self.uid = "tcp:<%s" % repr(addr)
        self.buffer = ""
        dbg(5, 'new Client child %s' % repr(addr))
        gate.on_connect(self, False)
      else:
        host, port, gate = args
        asyncore.dispatcher_with_send.__init__(self)
        self.isChild = False
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        addr = (host, port)
        self.connect( addr )
        self.gate = gate
        self.uid = "tcp:%s:%d" % (host, port)
        self.buffer = ""
        dbg(5, 'new Client %s ' % repr(addr))
    #
    @classmethod
    def new(cls, host, port, gate):
      return cls(False, (host, port, gate))
    #
    @classmethod
    def subnew(cls, sock, addr, gate):
      return cls(True, (sock, addr, gate))
    #
    def queue(self, data):
      self.buffer += data
    #
    def handle_connect(self):
      dbg(5, 'Connected')
      self.gate.on_connect(self, True)
    #
    def handle_error(self):
      nil, t, v, tbinfo = asyncore.compact_traceback()
      # sometimes a user repr method will crash.
      try:
        self_repr = repr(self)
      except:
        self_repr = '<__repr__(self) failed for object at %0x>' % id(self)
      dbg(5, 'Error: uncaptured python exception, closing channel %s (%s:%s %s)' % (
        self_repr, t, v, tbinfo
      ))
      #
      self.close()
      self.gate.state = 0 # reconnect
    #
    def handle_close(self):
      self.close()
      self.gate.state = 0 # reconnect
      handler.gate.on_disconnect(self)
    #
    def handle_read(self):
      data = self.recv(0x10000)
      self.gate.on_data(self, data)
    #
    def writable(self):
#      dbg(5, "Need sent %d bytes" % len(self.buffer))
      return (len(self.buffer) > 0)
    #
    def handle_write(self):
      sent = self.send(self.buffer)
      dbg(5, "Sent %d bytes" % sent)
      self.buffer = self.buffer[sent:]
    #
  #
  class Server(asyncore.dispatcher):
    gate = None
    #
    def __init__(self, host, port, gate):
      asyncore.dispatcher.__init__(self)
      self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
      self.set_reuse_addr()
      self.bind( (host, port) )
      self.listen(5)
      self.gate = gate
    #
    def handle_accept(self):
      pair = self.accept()
      if pair is not None:
        sock, addr = pair
        handler = self.gate.Client.subnew(sock, addr, self.gate)
    #
  #
  #
  mode = None
  addr = None
  port = None
  #
  socket = None
  clients = None
  state = None
  sm = None
  #
  def __init__(self, mode, addr, port):
    self.mode = mode
    self.addr = addr
    self.port = port
    self.socket = None
    self.clients = []
    self.state = 0
    if mode == 'connect':
      self.sm = self.sm_connect
    elif mode == 'listen':
      self.sm = self.sm_listen
    else:
      raise Exception('Bad Gate_TCP mode '+mode)
  #
  def tick(self, tick):
    return self.sm(tick)
    pass
  #
  def sm_connect(self, tick):
    if self.state==0:
      self.socket = self.Client.new(self.addr, self.port, self)
      self.state=1
    elif self.state==1:
      pass
    pass
  #
  def sm_listen(self, tick):
    if self.state==0:
      self.socket = self.Server(self.addr, self.port, self)
      self.state=1
    elif self.state==1:
      pass
    pass
  #
  #
  def on_connect(self, cli, isCli):
    self.clients.append(cli)
    dbg(4, "on_connect %s %s" % (repr(cli), repr(isCli)) )
    if isCli:
      cli.queue( "RAW32\n" )
  #
  def on_disconnect(self, cli):
    self.clients.remove(cli)
  #
  def on_data(self, cli, data):
    #self.node.on_message(cli.uid, -1, data)
    dbg(5, "data['%s' '%s']" % (cli.uid, data))



#-------------------------------------------------------------
#-------------------------------------------------------------

tick = 0.1 # 10 ms
node1 = Node('Node_1')
node2 = Node('Node_2')

node1.gate_add( Gate_TCP('connect', 'localhost', 7000) )
node2.gate_add( Gate_TCP('listen', 'localhost', 7000) )

while True:
  asyncore.loop(0, 1, None, 10)
  node1.tick(tick)
  node2.tick(tick)
  time.sleep(tick)
  sys.stderr.write('.')

