import time
import sys
import weakref
import struct
import zlib

from common import *
from l_common import *
from l_channel import *
from l_retry import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Bus(NodeObject, RetryQueue):
  #
  class SysCommand(object):
    bus = None
    key = None
    chkey = None
    command = None
    args = None
    #
    #
    def __init__(self, bus, chkey, key, value):
      self.bus = bus
      self.key = int(key)
      self.chkey = chkey
      self.command, self.args = next_word(value)
    #
    def ack(self, ec=None):
      bus = self.bus
      frame = bus.syscmd_emit_ack(self.key, ec)
      bus.sysaq[self.key] = [bus.systimeleave, frame]
    #
    def nak(self, ec):
      bus = self.bus
      if ec is None:
        frame = bus.syscmd_emit_nak(self.key, 'BAD_COMMAND')
      else:
        frame = bus.syscmd_emit_nak(self.key, ec)
      bus.sysaq[self.key] = [bus.systimeleave, frame]
    #
  #
  queue = None
  #
  gate = None
  addr = None
  aux = None
  other_uid = None
  #
  frameq = None
  #
  def __init__(self, gate, addr, aux):
    NodeObject.__init__(self, None)
    RetryQueue.__init__(self)
    #
    self.gate = gate
    self.addr = addr
    self.aux = aux
    #
    self.uid = "%s::%s" % (gate.uid, addr)
    self.sid = 0
    #
    self.frameq = []
  #
  def syscmd_emit_(self, command, args=None, callback=None, sid=None):
    # <sid> :: <command> <args>
    isys = sid is not None
    if args is not None:
      if is_array(args):
        args = ' '.join(args)
      command = "%s %s" % (command, args)
    #
    if isys:
      item = RetryQueueInItem((command, callback), sid)
      self.rq_in_add(item, True)
    else:
      item = RetryQueueOutItem((command, callback))
      self.rq_out_add(item, True)
    #
    return item.aux
  #
  def syscmd_emit_ack(self, sid, ext=None):
    return self.syscmd_emit_('ACK', ext, None, sid)
  #
  def syscmd_emit_nak(self, sid, ext=None):
    return self.syscmd_emit_('NAK', ext, None, sid)
  #
  def syscmd_emit(self, command, args=None, callback=None, transpr=None):
    if not transpr:
      if self.other_uid is None:
        return # can't emit this frame
    #
    return self.syscmd_emit_(command, args, callback, None)
  #
  #
  def syscmd_emit_nak(self, sid, ext=None):
    frame = self.syscmd_make('NAK', ext, None, sid)
    self.frame_emit(frame)
    return frame
  #
  def on_sysframe(self, frame):
    self.frameq.append(frame)
  #
  def on_tick(self, tick):
    #
    # exec frames
    while len(self.frameq):
      f = self.frameq.pop(0)
      self.exec_sysframe(f[0], f[2], f[3], f[4])
    #
    self.on_rq_tick(self)
    #
    pass
  #
  def exec_sysframe(self, sysflag, chkey, key, value):
    try:
      sc = self.SysCommand(self, chkey, key, value)
    except:
      self.d_warning("Bad syscmd %s", repr((sysflag, chkey, key, value)))
      return
    #
    if sysflag:
      if sc.command=='ACK':
        self.on_rq_out_done(sc.key, (True, sc.args))
      elif sc.command=='NAK':
        self.d_warning("got NAK %s", sc.args)
        self.on_rq_out_done(sc.key, (False, sc.args))
      else:
        self.d_warning("Damaged syscmd %s", repr((sysflag, chkey, key, value)))
      return
    elif sc.key in self.sysaq:
      self.d_debug("dup syscmd %s", repr((sysflag, chkey, key, value)))
      self.frame_emit(self.sysaq[sc.key][1])
      return # dup packet, skipped
    else:
      self.d_debug("syscmd{%s %s %s %s}", repr(sc.key), repr(sc.chkey), repr(sc.command), repr(sc.args))
      if self.on_syscmd(sc) is None:
        syscmd.nak(None)
    pass
  #
  #
  def on_rq_out_done(self, item, uid, data):
    if item is None:
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
    item.aux = make_frame(False, 0, '', "%d" % item.uid, item.data[0])
  #
  def on_rq_in_add(self, item):
    item.aux = make_frame(True, 0, '', "%d" % item.uid, item.data[0])
  #
  def on_rq_out_send(self, item):
    self.gate.frame_emit(self.addr, self.aux, item.aux)
  #
  #
  def on_rq_in_send(self, item):
    self.gate.frame_emit(self.addr, self.aux, item.aux)
  #
  def on_rq_in_lost(self, item):
    return # do nothing
  #
  def on_rq_out_lost(self, item):
    self.d_warning("Undelivered command %s %s", item.uid, item.data[1])
    self.on_rq_out_done(sc.key, (None, None))
  #
  #
  def on_syscmd(self, sc):
    if sc.command=='G_OPEN':
      return self.on_syscmd_gate_open(sc)
    #
    if self.other_uid is None:
      sc.nak('GATE_DISCONNECTED')
      return True
    elif sc.command=='G_CLOSE':
      return self.on_syscmd_gate_close(sc)
    #
    elif sc.command=='S_SCAN':
      return self.on_syscmd_service_scan(sc)
    elif sc.command=='S_SCAN_RESULT':
      return self.on_syscmd_service_scanresult(sc)
    #
    elif sc.command=='C_OPEN':
      return self.on_syscmd_channel_open(sc)
    elif sc.command=='C_OPENED':
      return self.on_syscmd_channel_opened(sc)
    elif sc.command=='C_CLOSE':
      return self.on_syscmd_channel_close(sc)
    #
    else:
      self.d_warning("unknown syscmd {%s %s}", sc.command, sc.args)
      pass
    #
  #
  def on_syscmd_gate_open(self, syscmd):
    argv, args = next_words(syscmd.args, 1)
    if argv is None:
      return
    self.other_uid = argv[0]
    syscmd.ack()
    return True
  #
  def on_syscmd_gate_close(self, syscmd):
    syscmd.ack()
    return True
    #
  #
  def on_syscmd_service_scan(self, syscmd):
    argv, args = next_words(syscmd.args, 2, 100)
    if argv is None:
      return
    syscmd.ack()
    #
    ret = self.node().service_on_scan([argv[0], argv[1]], argv[2:])
    self.syscmd_emit('S_SCAN_RESULT', ' '.join(ret), True)
    return True
  #
  def on_syscmd_service_scanresult(self, syscmd):
    argv, args = next_words(syscmd.args, 3, 100)
    if argv is None:
      return
    syscmd.ack()
    #
    for sid in argv[3:]:
      self.node().service_on_scanresult(argv[1], argv[0], argv[2], sid)
    return True
  #
  def on_syscmd_channel_open(self, syscmd):
    argv, args = next_words(syscmd.args, 5, 100, (int, None, None, None, None))
    if argv is None:
      return
    return self.node().channel_on_open(syscmd, argv)
  #
  def on_syscmd_channel_opened(self, syscmd):
    argv, args = next_words(syscmd.args, 2, 100, (int,int))
    if argv is None:
      return
    return self.node().channel_on_opened(syscmd, argv)
  #
  def on_syscmd_channel_close(self, bus, sid, chkey, command, args):
    argv, args = next_words(args, 2, 100, (int,))
    if argv is None:
      return
    return self.node().channel_on_close(syscmd, argv)
  #


#-------------------------------------------------------------
#-------------------------------------------------------------

