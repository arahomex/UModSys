#!/usr/bin/python -B

import time
import sys
import socket
import select
import errno
import asyncore
import weakref

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
  def make_frame(self, chid, chkey, , payload)
  #
  def on_gate_connected(self, gate, addr):
    dbg(3, "on_gate_connected %s %s" % (gate.uid, addr) )
    frame = self.make_frame(
    pass
  #
  def on_gate_disconnected(self, gate, addr):
    dbg(3, "on_gate_disconnected %s %s" % (gate.uid, addr) )
    pass
  #
  def on_message(self, addr, channel, payload):
    pass

#-------------------------------------------------------------

class Gate(object):
  #
  uid = None
  node = None
  #
  pass

class Gate_TCP(Gate):
  class Client(asyncore.dispatcher):
    gate = None
    uid = None
    is_child = False
    is_connected = False
    tmode = None
    #
    writebuf = None
    readbuf = None
    #
    def __init__(self, is_child, sock, gate, addr):
      asyncore.dispatcher.__init__(self, sock)
      self.is_child = is_child
      self.gate = gate
      self.writebuf = ''
      self.readbuf = ''
      self.need_connect = not is_child
      if is_child:
        self.uid = "tcp:<%s" % repr(addr)
        self.tmode = None
        self.is_connected = True
      else:
        self.uid = "tcp:>%s" % repr(addr)
        self.tmode = 'RAW32'
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connect( addr )
      dbg(5, 'new Client %s' % self.uid)
    #
    @classmethod
    def new(cls, host, port, gate):
      return cls(False, None, gate, (host, port))
    #
    @classmethod
    def subnew(cls, sock, addr, gate):
      return cls(True, sock, gate, addr)
    #
    def queue(self, data):
      self.writebuf += data
    #
    def set_tmode(self, tmode):
      if (tmode=='RAW32') and (self.tmode is None):
        self.tmode = tmode
        dbg(5, "%s::transmit_mode %s" % (self.uid, repr(tmode)))
        if self.is_child:
          self.gate.on_connect(self)
      else:
        raise Exception('Bad transmit mode %s' % tmode)
    #
    def handle_connect(self):
      if self.is_connected:
        return
      self.is_connected = True
      if not self.is_child:
        self.need_tms = False
        self.queue( self.tmode + "\n" )
      self.gate.on_connect(self)
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
      if self.is_connected:
        self.gate.on_disconnect(self, True)
      if not self.is_child:
        self.gate.on_reconnect(self)
    #
    def handle_close(self):
      self.close()
      if self.is_connected:
        self.gate.on_disconnect(self)
      if not self.is_child:
        self.gate.on_reconnect(self)
    #
    def handle_expt(self):
      pass
    #
    def handle_read(self):
      if not self.is_connected:
        self.handle_connect()
      #
      data = self.recv(0x10000)
      if data is not None:
        self.readbuf += data
        if self.tmode is None:
          pos = self.readbuf.find("\n")
          if pos!=-1:
            mode = self.readbuf[0:pos]
            #dbg(6, "%s::tmode %d '%s' %d '%s'" % (self.uid, len(self.readbuf), self.readbuf, pos, self.readbuf[pos+1:]))
            self.readbuf = self.readbuf[pos+1:]
            self.set_tmode(mode)
            if self.readbuf=='':
              return
          else:
            return
        self.gate.on_data(self)
    #
    def writable(self):
      if not self.is_connected:
        self.handle_connect()
      #
      l = len(self.writebuf)
      if l>0:
        dbg(5, "%s::Need sent %d bytes" % (self.uid, l))
      return (l > 0)
    #
    def handle_write(self):
      sent = self.send(self.writebuf)
      dbg(5, "Sent %s bytes" % repr(sent))
      if sent is not None:
        self.writebuf = self.writebuf[sent:]
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
  def __init__(self, uid, mode, addr, port):
    self.uid = uid
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
  def on_reconnect(self, cli):
    self.socket = None
    self.state = 0 # reconnect
    pass
  #
  def on_connect(self, cli):
    self.clients.append(cli)
    dbg(4, "on_connect %s" % cli.uid )
    self.node.on_gate_connected(self, cli.uid)
  #
  def on_disconnect(self, cli, isError=False):
    dbg(4, "on_disconnect %s %s" % (cli.uid, repr(isError)) )
    self.clients.remove(cli)
    self.node.on_gate_disconnected(self, cli.uid)
  #
  def on_data(self, cli):
    #self.node.on_message(cli.uid, -1, data)
    dbg(5, "data['%s' '%s']" % (cli.uid, cli.readbuf))
    cli.readbuf = ''



#-------------------------------------------------------------
#-------------------------------------------------------------

tick = 0.1 # 10 ms
node1 = Node('Node_1')
node2 = Node('Node_2')

node1.gate_add( Gate_TCP('client', 'connect', 'localhost', 7000) )
node2.gate_add( Gate_TCP('server', 'listen', 'localhost', 7000) )

while True:
#  asyncore.loop(0, 1, None, 10)
  asyncore.loop(0, 1, None, 1)
  node1.tick(tick)
  node2.tick(tick)
  time.sleep(tick)
  sys.stderr.write('.')

