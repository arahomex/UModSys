import time
import sys
import weakref
import struct
import zlib

from common import *

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
#-------------------------------------------------------------

