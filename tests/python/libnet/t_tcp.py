import time
import sys
import socket
import errno
import asyncore
import weakref
import struct
import zlib

from common import *
from l_common import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Gate_TCP(Gate):
  class Client(asyncore.dispatcher, BaseObject):
    gate = None
    uid = None
    is_child = False
    is_connected = False
    tmode = None
    #
    frames = None
    iframes = None
    rblock = None
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
      self.frames = []
      self.iframes = []
      if is_child:
        self.uid = "tcp:<%s" % repr(addr)
        self.tmode = None
        self.is_connected = True
      else:
        self.uid = "tcp:>%s" % repr(addr)
        self.tmode = 'RAW32'
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connect( addr )
      self.d_debug('new Client %s', self.uid)
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
    def add_frame(self, frame):
      self.frames.append(frame)
    #
    def on_tick(self, time):
      if len(self.frames):
        block = ''.join(self.frames)
        self.frames = []
        self.writebuf += struct.pack('!II', len(block), zlib.crc32(block) & 0xffffffff) + block
    #
    #
    def on_block(self, block):
      while True:
        block, frame = parse_frame(block)
        if frame is None:
          break
        self.d_debug("on_block %d %s", len(block), repr(block))
        self.gate.on_frame(self, frame)
      pass
    #
    def on_data(self):
      #self.d_debug("data[%s] %d %s", self.uid, rlen, repr(self.readbuf))
      while True:
        rlen = len(self.readbuf)
        if self.rblock is None:
          if rlen<8:
            return # incomplete
          self.rblock = struct.unpack('!II', self.readbuf[0:8])
          self.readbuf = self.readbuf[8:]
          rlen = len(self.readbuf)
        if rlen< self.rblock[0]:
          return # incomplete
        l, c = self.rblock
        self.rblock = None
        block = self.readbuf[0:l]
        self.readbuf = self.readbuf[l:]
        if (zlib.crc32(block) & 0xffffffff) == c:
          self.on_block(block)
        else:
          self.d_debug("block crc failed, skip it")
          pass
    #
    def set_tmode(self, tmode):
      if (tmode=='RAW32') and (self.tmode is None):
        self.tmode = tmode
        self.d_debug("transmit_mode %s", repr(tmode))
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
      self.d_error('Error: uncaptured python exception, closing channel %s (%s:%s %s)',
        self_repr, t, v, tbinfo
      )
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
            ## self.d_debug("tmode %d %s %d %s'", len(self.readbuf), repr(self.readbuf), pos, repr(self.readbuf[pos+1:]))
            self.readbuf = self.readbuf[pos+1:]
            self.set_tmode(mode)
            if self.readbuf=='':
              return
          else:
            return
        self.on_data()
    #
    def writable(self):
      if not self.is_connected:
        self.handle_connect()
      #
      l = len(self.writebuf)
      if l>0:
        self.d_debug("Need sent %d bytes", l)
      return (l > 0)
    #
    def handle_write(self):
      sent = self.send(self.writebuf)
      self.d_debug("Sent %s bytes", repr(sent))
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
  def __init__(self, node, uid, mode, addr, port):
    Gate.__init__(self, uid, node)
    #
    self.mode = mode
    self.addr = addr
    self.port = port
    self.socket = None
    self.clients = {}
    self.state = 0
    if mode == 'connect':
      self.sm = self.sm_connect
    elif mode == 'listen':
      self.sm = self.sm_listen
    else:
      raise Exception('Bad Gate_TCP mode '+mode)
  #
  def on_tick(self, tick):
    for cid, cli in self.clients.iteritems():
      cli.on_tick(tick)
    return self.sm(tick)
  #
  def frame_emit(self, addr, cli, frame):
    cli.add_frame(frame)
  #
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
    self.d_info("on_connect %s" % cli.uid )
    self.clients[cli.uid] = cli
    self.node().on_gate_connected(self, cli.uid, cli)
  #
  def on_disconnect(self, cli, isError=False):
    self.d_info("on_disconnect %s %s" % (cli.uid, repr(isError)) )
    if cli.uid in self.clients:
      del self.clients[cli.uid]
      self.node().on_gate_disconnected(self, cli.uid, cli)
  #
  def on_frame(self, cli, frame):
    self.d_debug("on_frame %s %s" % (cli.uid, repr(frame)) )
    self.node().on_gate_frame(self, cli.uid, frame)
  #


#-------------------------------------------------------------
#-------------------------------------------------------------

def transport_tcp_tick(tick):
#  asyncore.loop(0, 1, None, 10)
  asyncore.loop(0, 1, None, 1)
  #dbg(1, '#')

#-------------------------------------------------------------
#-------------------------------------------------------------


