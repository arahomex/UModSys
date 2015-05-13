#!/usr/bin/python -B

import time
import sys
import socket
import select
import errno
import asyncore

READ_ONLY = select.POLLIN | select.POLLPRI | select.POLLHUP | select.POLLERR
READ_WRITE = READ_ONLY | select.POLLOUT

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
  #
  def tick(self, tick):
    for gate in self.gates:
      gate.tick(tick)
  #

#-------------------------------------------------------------

class Gate(object):
  #
  pass

class Gate_TCP(Gate):
  class Client(asyncore.dispatcher):
    gate = None
    #
    def __init__(self, host, port, gate):
        asyncore.dispatcher.__init__(self)
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connect( (host, port) )
        self.gate = gate
        self.buffer = "TEST"
        print 'new Client'
    #
    def handle_connect(self):
        print 'Connected'
        pass
    #
    def handle_error(self):
        print 'Error'
        self.close()
        self.gate.state = 0 # reconnect
    #
    def handle_close(self):
        self.close()
        self.gate.state = 0 # reconnect
    #
    def handle_read(self):
        print self.recv(8192)
        self.close()
        self.gate.state = 0 # reconnect
    #
    def writable(self):
        return (len(self.buffer) > 0)
    #
    def handle_write(self):
        sent = self.send(self.buffer)
        self.buffer = self.buffer[sent:]
    #
  #
  class ServerClient(asyncore.dispatcher_with_send):
    gate = None
    #
    def handle_read(self):
        data = self.recv(8192)
        if data:
            self.send(data)
    def handle_close(self):
        self.close()
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
        print 'Incoming connection from %s' % repr(addr)
        handler = self.gate.ServerClient(sock)
        handler.gate = self.gate
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
    self.cliens = []
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
      self.socket = self.Client(self.addr, self.port, self)
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



#-------------------------------------------------------------
#-------------------------------------------------------------

tick = 1.0 # 10 ms
node1 = Node('Node_1')
node2 = Node('Node_2')

node1.gate_add( Gate_TCP('connect', 'localhost', 7000) )
node2.gate_add( Gate_TCP('listen', 'localhost', 7000) )

while True:
  asyncore.loop(0, 1, None, 10)
  node1.tick(tick)
  node2.tick(tick)
  time.sleep(tick)
  sys.stderr.write('.');

