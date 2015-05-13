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
  class Server(asyncore.dispatcher):
    def __init__(self, host, port):
      self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
      self.set_reuse_addr()
      self.bind((host, port))
      self.listen(5)
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
      #self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      #self.socket.setblocking(0)
      try:
        #self.socket.connect((self.addr, self.port)) # Expect socket.error: (115, 'Operation now in progress')
        pass
      except socket.error, msg:
        (errorno, errmsg) = msg
        #print >>sys.stderr, self.mode, msg, (errorno, errmsg), (errno.EALREADY)
        if errorno==errno.EALREADY:
          pass
        elif errorno==errno.EINPROGRESS or errorno==errno.EWOULDBLOCK:
          pass
        elif errorno == errno.EISCONN:
          pass
        else:
          raise
      self.state=1
    elif self.state==1:
      #r, w, x = select.select([self.socket], [self.socket], [self.socket], 0)
      #print >>sys.stderr, "%s: r=%s w=%s x=%s" % (self.mode, r, w, x)
      pass
    pass
  #
  def sm_listen(self, tick):
    if self.state==0:
      self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.socket.setblocking(0)
      self.socket.bind((self.addr, self.port))
      self.socket.listen(1)
      self.state=1
    elif self.state==1:
      r, w, x = select.select([self.socket], [self.socket], [self.socket], 0)
      print >>sys.stderr, "%s: r=%s w=%s x=%s" % (self.mode, r, w, x)
    pass



#-------------------------------------------------------------
#-------------------------------------------------------------

tick = 1.0 # 10 ms
node1 = Node('Node_1')
node2 = Node('Node_2')

node1.gate_add( Gate_TCP('connect', 'localhost', 7000) )
node2.gate_add( Gate_TCP('listen', 'localhost', 7000) )

while True:
  node1.tick(tick)
  node2.tick(tick)
  time.sleep(tick)
  sys.stderr.write('.');

