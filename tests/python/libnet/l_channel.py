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

class MetaChannel(object):
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
    elif mode & cls.F_SEQ:
      ret.append('SEQ')
    elif mode & cls.F_ORDERED:
      ret.append('ORDERED')
    elif mode & cls.F_PROTECT:
      ret.append('PROTECT')
    if len(ret)==0:
      ret.append('NONE')
    return ret
  #
  @classmethod
  def channel(cls, serv, chid, addr, mode, aux, chid2):
    ms = mode & cls.F_STREAM
    if ms == cls.F_STREAM:
      return cls.Stream(serv, chid, addr, mode, aux, chid2)
    elif ms & cls.F_RETRY:
      return cls.Retry(serv, chid, addr, mode, aux, chid2)
    elif ms & cls.F_ORDERED:
      return cls.Ordered(serv, chid, addr, mode, aux, chid2)
    else:
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

class Channel_Retry(Channel_Simple, RetryQueue):
  #
  def __init__(self, serv, chid, addr, mode, aux, chid2):
    ChannelSimple.__init__(self, serv, chid, addr, mode, aux, chid2)
    RetryQueue.__init__(self)
  #
  def send(self, value, key=''):
    hsq = self.mode & self.F_HAVE_SEQ
    if hsq:
      self.outseqn += 1
      key = ("%d " % self.outseqn) + key
      #
      if hsq & self.F_RETRY:
        self.outseq[self.outseqn] = [key, value, self.timenext, self.times]
    #
    frame = make_frame(False, self.other_uid, '', key, value)
    self.bus.frame_send(frame)
  #
  #
  #
  def on_tick(self, tick):
    if self.other_uid is not None:
      self.d_debug("Tick request send")
      self.service.on_send(self)
    #
    while len(self.readq):
      key, value = self.readq.pop(0)
      self.service.on_receive(self, key, value)
    #
    if tick:
      acks = []
      for k in self.inseq.keys():
        v = self.inseq[k]
        if v[1]:
          acks.append(k)
        v[0] = v[0] - tick
        if v[0]<0:
          del self.inseq[k]
      for k in self.outseq.keys():
        pass
      if len(acks):
        frame = make_frame(True, self.other_uid, '', '', "\n".join(acks))
        self.bus.frame_emit(frame)
    #
  #
  def on_frame(self, sysflag, chkey, key, value):
    hsq = self.mode & self.F_HAVE_SEQ
    #
    if sysflag:
      if hsq:
        acks = value.split("\n")
        for v in acks:
          if v in self.outseq:
            del self.outseq[v]
    #
    if self.sysvals:
      argv, key = next_words(key, self.sysvals, 0, self.sysints)
      #
      if argv is None:
        return False
      #
      if hsq:
        pid = argv.pop(0)
        #
        if hsq == self.F_RETRY:
          # send ack, store id
          if pid in self.inseq:
            self.inseq[pid] = [self.timeleave, True]
            return False # do not receive
        #
        if hsq == self.F_SEQ:
          if pid<=self.inseqn:
            return False # do not receive
          self.inseqn = pid
    #
    self.readq.append((key, value))
    return True
  #
  #
  def on_rq_out_done(self, item, uid, data):
    if item is not None:
      cb = item.data[1]
      if cb is None:
        pass
      elif type(cb) is bool:
        pass
      else:
        cb(item, data[0], data[1]) # item, succeeded, errorcode
    else:
      self.d_warning("syscmd %d already done", uid)
  #
  def on_rq_out_add(self, item):
    key, value = item.data
    item.data = None
    item.aux = make_frame(False, self.other_uid, '', ("%d" % item.uid)+key, value)
  #
  def on_rq_out_send(self, item):
    self.bus.frame_send(item.aux)
  #
  def on_rq_out_lost(self, item):
    self.d_warning("Undelivered packet %d", item.uid)
    self.rq_out_done(sc.key, (None, None))
  #
  #
  def on_rq_in_add(self, item):
    data = item.data
    item.data = None
    item.aux = make_frame(True, self.other_uid, '', "%d" % item.uid, data)
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
MetaChannel.Retry = Channel_Retry
#MetaChannel.Ordered = Channel_Ordered
#MetaChannel.Stream = Channel_Stream

#-------------------------------------------------------------
#-------------------------------------------------------------

