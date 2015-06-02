import time
import sys
import weakref
import struct
import zlib

from common import *
from l_common import *
from l_retry import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class MetaChannel(BaseObject):
  F_RETRY   = 0x01
  F_SEQ     = 0x02
  F_ORDERED = 0x04
  F_STREAM  = 0x07
  F_PROTECT = 0x20
  #
  Simple = None
  Retry = None
  Ordered = None
  Stream = None
  #
  @classmethod
  def mode_decode(cls, mode):
    rv = 0
    i = 0
    for m in mode:
      i = i + 1
      if m=='SEQ':
        rv = rv | cls.F_SEQ
      elif m=='RETRY':
        rv = rv | cls.F_RETRY | cls.F_SEQ
      elif m=='ORDERED':
        rv = rv | cls.F_ORDERED | cls.F_SEQ
      elif m=='PROTECT':
        rv = rv | cls.F_PROTECT
      elif m=='STREAM':
        rv = rv | cls.F_STREAM
      elif (m=='') or (m=='NONE'):
        pass
      elif m=='-':
        break # start custom options
      else:
        cls.d_cwarning('Invalid mode atom %s', repr(m))
        return (None, None) # error
    return (rv, mode[i:])
  #
  @classmethod
  def mode_encode(cls, mode):
    ret = []
    if mode & cls.F_RETRY:
      ret.append('RETRY')
    if mode & cls.F_SEQ:
      ret.append('SEQ')
    if mode & cls.F_ORDERED:
      ret.append('ORDERED')
    if mode & cls.F_PROTECT:
      ret.append('PROTECT')
    if len(ret)==0:
      ret.append('NONE')
    return ret
  #
  @classmethod
  def channel(cls, serv, chid, addr, mode, aux, chid2):
    ms = mode & cls.F_STREAM
    if ms == cls.F_STREAM:
      cls.d_cinfo('Creating STREAM channel')
      return cls.Stream(serv, chid, addr, mode, aux, chid2)
    elif ms & cls.F_RETRY:
      cls.d_cinfo('Creating RETRY channel')
      return cls.Retry(serv, chid, addr, mode, aux, chid2)
    elif ms & cls.F_ORDERED:
      cls.d_cinfo('Creating ORDERED channel')
      return cls.Ordered(serv, chid, addr, mode, aux, chid2)
    else:
      cls.d_cinfo('Creating SIMPLE channel')
      return cls.Simple(serv, chid, addr, mode, aux, chid2)
    pass
  #

#-------------------------------------------------------------

class Channel_Simple(NodeObject, MetaChannel):
  #
  service = None
  addr = None # (nid, sid, func)
  mode = 0
  aux = None
  other_uid = None
  bus = None
  readq = None
  #
  def __init__(self, serv, chid, addr, mode, aux, chid2):
    NodeObject.__init__(self, chid)
    #
    self.mode = mode
    self.aux = aux
    self.other_uid = chid2
    self.service = serv
    self.addr = addr
    self.readq = []
  #
  def connect(self):
    self.node().channel_initate(self.addr, self.uid, self.other_uid, MetaChannel.mode_encode(self.mode), self.aux, True)
  #
  def disconnect(self):
    if self.other_uid is not None:
      self.node().channel_initate(self.addr, self.uid, self.other_uid, self.options, False)
  #
  def send(self, value, key=''):
    frame = make_frame(False, self.other_uid, '', key, value)
    self.bus.frame_send(frame)
  #
  def on_frame(self, sysflag, chkey, key, value):
    if sysflag:
      self.d_warning('No sysmsg allowed for the channel')
      return
    #
    self.readq.append((key, value))
    return True
  #
  def on_tick(self, tick):
    if self.other_uid is not None:
      self.d_debug("Tick request send")
      self.service.on_send(self)
    #
    while len(self.readq):
      key, value = self.readq.pop(0)
      self.service.on_receive(self, value, key)
  #

#-------------------------------------------------------------

class Channel_Ordered(Channel_Simple):
  #
  in_num = None
  out_num = None
  #
  def __init__(self, serv, chid, addr, mode, aux, chid2):
    Channel_Simple.__init__(self, serv, chid, addr, mode, aux, chid2)
    #
    self.in_num = 0
    self.out_num = 0
  #
  def send(self, value, key=''):
    self.out_num = self.out_num + 1
    key = ("%d " % self.out_num) + key
    return Channel_Simple.send(self, value, key)
  #
  def on_frame(self, sysflag, chkey, key, value):
    if not sysflag:
      uid, key = next_word(key)
      if key is None: 
        self.d_warning('Bad packet uid')
        return
      uidn = as_integer(uid)
      if uidn is None: 
        self.d_warning('Invalid packet uid %s', uid)
        return
      if uidn<=self.out_num:
        self.d_info('Skip unordered packet %d, need >%d', uidn, self.out_num)
        self.out_num = uidn
        return
    #
    self.readq.append((key, value))
    return True
  #

#-------------------------------------------------------------

class Channel_Retry(Channel_Simple, RetryQueue):
  #
  def __init__(self, serv, chid, addr, mode, aux, chid2):
    Channel_Simple.__init__(self, serv, chid, addr, mode, aux, chid2)
    RetryQueue.__init__(self)
  #
  def send(self, value, key=''):
    item = RetryQueueOutItem((key, value))
    self.rq_out_add(item, True)
  #
  def on_tick(self, tick):
    Channel_Simple.on_tick(self, tick)
    self.on_rq_tick(tick)
  #
  def on_frame(self, sysflag, chkey, key2, value):
    uid, key = next_word(key2)
    if key is None: 
      self.d_warning('Bad packet uid of "%s"', key2)
      return
    uidn = as_integer(uid)
    if uidn is None: 
      self.d_warning('Invalid packet uid %s', uid)
      return
    #
    if sysflag:
      self.rq_out_done(uidn, True)
      return True
    #
    item = RetryQueueInItem(('', key, value))
    self.rq_in_add(item, uidn, True)
    return True
  #
  #
  def on_rq_out_done(self, item, uid, data):
    pass
  #
  def on_rq_out_add(self, item):
    key, value = item.data
    item.data = None
    item.aux = make_frame(False, self.other_uid, '', ("%d " % item.uid)+key, value)
  #
  def on_rq_out_send(self, item):
    self.bus.frame_send(item.aux)
  #
  def on_rq_out_lost(self, item):
    self.d_warning("Undelivered packet %d", item.uid)
    self.rq_out_done(uid, None)
  #
  #
  def on_rq_in_add(self, item):
    data, key, value = item.data
    item.data = None
    self.readq.append((key, value))
    item.aux = make_frame(True, self.other_uid, '', "%d " % item.uid, data)
  #
  def on_rq_in_dup(self, item, dup):
    self.bus.frame_send(dup.aux)
  #
  def on_rq_in_send(self, item):
    self.bus.frame_send(item.aux)
  #
  def on_rq_in_lost(self, item):
    return # do nothing
  #


#-------------------------------------------------------------

MetaChannel.Simple = Channel_Simple
MetaChannel.Ordered = Channel_Ordered
MetaChannel.Retry = Channel_Retry
#MetaChannel.Stream = Channel_Stream

#-------------------------------------------------------------
#-------------------------------------------------------------

