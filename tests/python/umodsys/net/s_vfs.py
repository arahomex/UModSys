import time
import sys
import weakref

from common import *
from l_common import *

from ..media.logic import Library

#-------------------------------------------------------------
#-------------------------------------------------------------

class Service_VFS_Server(Service):
  #
  vfs = None
  pwd = None
  #
  def __init__(self, node, vfs, pwd):
    Service.__init__(self, "vfs:%s" % random_key(4), node)
    #
    self.vfs = vfs
    self.pwd = vfs.point_normdir(pwd)
  #
  def on_scan(self, kw):
    if kw=='vfs':
      return True
    return False
  #
  def on_receive(self, ch, value, key):
    self.d_debug("receive %s %s", key, repr(value))
    kw, inf = next_word(key, ':')
    if kw == '+list':
      names = []
      for fn in self.vfs.list(self.pwd+'*'):
        names.append(fn[len(self.pwd):])
      ch.send("\n".join(names), 'list:')
    elif kw == '+file':
      md5 = self.vfs.hash_md5(self.pwd+inf)
      if md5 is None:
        self.d_error("Failed get hash %s", inf)
        ch.send('', 'nofile:'+inf)
      elif md5!=value:
        data = self.vfs.load(self.pwd+inf)
        if data is None:
          self.d_error("Failed get file %s", inf)
          ch.send('', 'nofile:'+inf)
        else:
          ch.send(data, 'file:'+inf)
      else:
          self.d_warning("Already get file %s", inf)
          ch.send('', 'nofile:'+inf)
    else:
      pass
  #
  def on_channel(self, sid, func, modeid, options):
    if func=='vfs':
      return True
    return False
  #
  pass

#-------------------------------------------------------------

class Service_VFS_Client(Service):
  #
  cache = None
  in_sync = None
  in_data = None
  in_list = None
  in_seq_max = 10
  #
  def __init__(self, node, cache, pwd):
    Service.__init__(self, "ping:%s" % random_key(4), node)
    #
    self.cache = cache
    self.pwd = cache.point_normdir(pwd)
    self.in_sync = None
  #
  def on_send(self, ch):
    if self.in_sync == 'list':
      ch.send('', '+list:')
      self.in_sync = '+list'
    elif self.in_sync == 'file':
        self.in_data = 0
        for i in range(1, self.in_seq_max):
          if len(self.in_list)==0:
            self.in_sync = None
          else:
            fn = self.in_list.pop(0)
            md5 = self.cache.hash_md5(self.pwd+fn)
            if md5 is None: md5 = ''
            ch.send(md5, '+file:'+fn)
            self.in_data = self.in_data + 1
        #
        self.in_sync = '+file'
    else:
      pass
    pass
  #
  def on_receive(self, ch, value, key):
    self.d_debug("receive %s %s", key, repr(value))
    kw, inf = next_word(key, ':')
    if kw=='list':
      self.in_sync = 'file'
      self.in_data = 0
      self.in_list = value.split("\n")
    elif kw=='file':
      self.in_data = self.in_data - 1
      if self.in_data<=0:
        self.in_sync = 'file'
      self.d_info('Writing file %s %d bytes', inf, len(value))
      self.cache.save(self.pwd+inf, value)
    elif kw=='nofile':
      self.in_data = self.in_data - 1
      if self.in_data<=0:
        self.in_sync = 'file'
      self.d_info('Already done file %s', inf)
    else:
      pass
    #self.npong = self.npong + 1
  #
  def on_connect(self, ch):
    self.d_info("channel open %d->%d", ch.uid, ch.other_uid)
  #
  def on_disconnect(self, ch, ec):
    self.d_info("channel close %d %s", ch.uid, repr(ec))
  #
  #
  def target(self, nid, sid, mode=''):
    ch = self.node().channel_open(self, nid, sid, 'vfs', mode)
    pass
  #
#  def statistics(self):
#    return "ping %d pong %d" % (self.nping, self.npong)
  #
  def sync(self):
    self.in_sync = 'list'
  #
  pass

#-------------------------------------------------------------
#-------------------------------------------------------------

