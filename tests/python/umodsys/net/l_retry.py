import time
import sys
import weakref
import struct
import zlib

from common import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class RetryQueueInItem(object):
  uid = None
  time = None
  data = None
  aux = None
  need_send = False
  #
  def __init__(self, d, uid=None):
    self.uid = uid
    self.data = d
  #

#-------------------------------------------------------------

class RetryQueueOutItem(object):
  uid = None
  time = None
  times = None
  data = None
  aux = None
  #
  def __init__(self, d, uid=None):
    self.uid = uid
    self.data = d
  #

#-------------------------------------------------------------

class RetryQueue:
  #
  in_q = None # in queue (normal)
  out_q = None # out queue (normal)
  #
  in_n = None
  out_n = None
  #
  in_time = 1.0
  out_time = 0.2
  out_times = 10
  #
  def __init__(self):
    self.in_n = 0
    self.out_n = 0
#    self.in_time = it
#    self.out_time = ot
#    self.out_times = otn
    self.in_q = {}
    self.out_q = {}
  #
  def rq_out_done(self, uid, data):
    if uid in self.out_q:
      item = self.out_q[uid]
      del self.out_q[uid]
      self.on_rq_out_done(item, uid, data)
    else:
      self.on_rq_out_done(None, uid, data)
  #
  def rq_out_add(self, item, isSend):
    self.out_n = self.out_n + 1
    item.uid = self.out_n
    item.time = self.out_time
    item.times = self.out_times
    #
    rv = self.on_rq_out_add(item)
    if rv:
      return rv
    #
    if isSend:
      rv = self.on_rq_out_send(item)
      if rv:
        return rv
      item.need_send = False
    self.out_q[item.uid] = item
    return
  #
  def rq_in_add(self, item, in_num, isSend):
    if in_num is None:
      self.in_n = self.in_n + 1
      in_num = self.in_n
    item.uid = 	in_num
    item.time = self.in_time
    #
    if in_num in self.in_q:
      dup = self.in_q[in_num]
      rv = self.on_rq_in_dup(item, dup)
      if rv:
        return rv
      return dup
    #
    rv = self.on_rq_in_add(item)
    if rv:
      return rv
    #
    if isSend:
      rv = self.on_rq_in_send(item)
      if rv:
        return rv
    self.in_q[item.uid] = item
    return
  #
  def on_rq_tick(self, tick):
    if tick<=0:
      return
    #
    for k in self.in_q.keys():
      item = self.in_q[k]
      if item.need_send:
        item.need_send = False
        rv = self.on_rq_in_send(item)
        if rv:
          return rv
      #
      item.time = item.time - tick
      if item.time<0:
        rv = self.on_rq_in_lost(item)
        if rv:
          return rv
        del self.in_q[k]
      #
    #
    for k in self.out_q.keys():
      item = self.out_q[k]
      item.time = item.time - tick
      if item.time<0:
        item.time = self.out_time
        item.times = item.times - 1
        if item.times<0:
          rv = self.on_rq_out_lost(item)
          if rv:
            return rv
          del out_q[k]
          continue
        rv = self.on_rq_out_send(item)
        if rv:
          return rv
  #
  #
  def on_rq_in_add(self, item):
    return
  #
  def on_rq_in_lost(self, item):
    return
  #
  def on_rq_in_dup(self, item, dup):
    return
  #
  def on_rq_in_send(self, item):
    return
  #
  #
  def on_rq_out_add(self, item):
    return
  #
  def on_rq_out_lost(self, item):
    return
  #
  def on_rq_out_send(self, item):
    return
  #
  def on_rq_out_done(self, item, uid, data):
    return
  #
  pass

#-------------------------------------------------------------
#-------------------------------------------------------------

