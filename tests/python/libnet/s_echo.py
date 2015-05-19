import time
import sys
import weakref
import struct
import zlib

from common import *
from l_common import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Service_Echo(Service):
  #
  def __init__(self, node):
    Service.__init__(self, "echo:%s" % random_key(4), node)
    #
  #
  def on_scan(self, kw):
    if kw=='echo':
      return True
    return False
  #
  def on_receive(self, ch, key, value):
    ch.send_pair(key, value)
  #
  def on_channel(self, sid, func, options):
    if func=='echo':
      return True
    return False
  #
  pass

#-------------------------------------------------------------

class Service_Ping(Service):
  #
#  ch = None
  data = None
  no = None
  #
  def __init__(self, node, data='ping'):
    Service.__init__(self, "ping:%s" % random_key(4), node)
    self.data = data
    self.no = 0
    #
  #
  def on_send(self, ch):
    self.no = self.no+1
    key = str(self.no)
    self.d_info("ping %s %s", key, repr(self.data))
    ch.send_pair(key, self.data)
  #
  def on_receive(self, ch, key, value):
    self.d_info("pong %s %s", key, repr(value))
  #
  def on_connect(self, ch):
    self.d_info("channel open %d->%d", ch.uid, ch.other_uid)
  #
  def on_disconnect(self, ch, ec):
    self.d_info("channel close %d %s", ch.uid, repr(ec))
  #
  #
  def target(self, nid, sid):
    ch = self.node().channel_open(self, nid, sid, 'echo', '')
    pass
  #
  pass

#-------------------------------------------------------------
#-------------------------------------------------------------

