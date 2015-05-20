import time
import sys
import weakref
import struct
import zlib

from common import *
from l_common import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Channel(NodeObject):
  #
  F_RETRY = 0x01
  F_SEQ = 0x02
  F_HUGE = 0x10
  F_PROTECT = 0x20
  #
  service = None
  addr = None # (nid, sid, func)
  mode = 0
  aux = None
  other_uid = None
  #
  bus = None
  #
  readq = None
  #
  @classmethod
  def mode_decode(cls, mode):
    rv = 0
    i = 0
    for m in mode:
      i = i + 1
      if m=='RETRY':
        rv = rv | cls.F_RETRY
      elif m=='SEQ':
        rv = rv | cls.F_SEQ
      elif m=='HUGE':
        rv = rv | cls.F_HUGE
      elif m=='PROTECT':
        rv = rv | cls.F_PROTECT
      elif m=='STREAM':
        rv = rv | cls.F_RETRY | cls.F_SEQ
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
    elif mode & cls.F_HUGE:
      ret.append('HUGE')
    elif mode & cls.F_PROTECT:
      ret.append('PROTECT')
    if len(ret)==0:
      ret.append('NONE')
    return ret
  #
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
    self.node().channel_initate(self.addr, self.uid, self.other_uid, Channel.mode_encode(self.mode), self.aux, True)
  #
  def disconnect(self):
    if self.other_uid is not None:
      self.node().channel_initate(self.addr, self.uid, self.other_uid, self.options, False)
  #
  def send(self, value, key=''):
    frame = make_frame(False, self.other_uid, '', key, value)
    self.bus.frame_emit(frame)
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
  #
  def on_frame(self, sysflag, chkey, key, value):
    self.readq.append((key, value))
  #

#-------------------------------------------------------------
#-------------------------------------------------------------

