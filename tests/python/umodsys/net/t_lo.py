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

class Gate_Loopback(Gate):
  #
  loopback = {} # static variable
  #
  target = None
  connected = None
  #
  #
  def __init__(self, node, uid, target):
    Gate.__init__(self, uid, node)
    #
    self.target = target
    self.connected = {}
    #
    if self.uid in self.loopback:
      raise Exception('Duplicate loopback uid '+uid)
    self.loopback[self.uid] = weakref.ref(self)
  #
  def on_tick(self, tick):
    if self.target is not None:
      self.frame_emit(self.target, None, None)
    return
  #
  def frame_emit(self, addr, cli, frame):
    if addr not in self.loopback:
      return False
    self.loopback[addr]().on_frame(self, frame)
  #
  #
  def on_frame(self, other, frame0):
    #self.d_info("on_disconnect %s %s", cli.uid, repr(isError))
    #self.node().on_gate_disconnected(self, cli.uid, cli)
    #
    if other.uid not in self.connected:
      self.connected[other.uid] = True
      self.node().on_gate_connected(self, other.uid, None)
    #
    if frame0 is None:
      return
    block, frame = parse_frame(frame0)
    self.d_debug("on_frame %s %s", other.uid, repr(frame))
    self.node().on_gate_frame(self, other.uid, frame)
  #


#-------------------------------------------------------------
#-------------------------------------------------------------


