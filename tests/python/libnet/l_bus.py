import time
import sys
import weakref
import struct
import zlib

from common import *
from l_common import *
from l_channel import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Bus(NodeObject):
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
      self.bus.syscmd_emit_ack(True, ec)
    #
    def nak(self, ec):
      if ec is None:
        self.bus.syscmd_emit_nak(False, 'BAD_COMMAND')
      else:
        self.bus.syscmd_emit_nak(False, ec)
    #
  #
  sid = None
  queue = None
  #
  gate = None
  addr = None
  aux = None
  other_uid = None
  #
  sysq = None
  frameq = None
  #
  systimenext = 0.5
  systimes = 2
  #
  def __init__(self, gate, addr, aux):
    NodeObject.__init__(self, None)
    #
    self.gate = gate
    self.addr = addr
    self.aux = aux
    #
    self.uid = "%s::%s" % (gate.uid, addr)
    self.sid = 0
    #
    self.sysq = {}
    self.frameq = []
  #
  def next_sid(self, callback=None):
    self.sid = sid = self.sid+1
    return sid
  #
  def syscmd_make(self, command, args=None, callback=None, sid=None, transpr=None):
    # <sid> :: <command> <args>
    isys = sid is not None
    if not isys:
      sid = self.next_sid()
    if args is not None:
      if is_array(args):
        args = ' '.join(args)
      command = "%s %s" % (command, args)
    frame = make_frame(isys, 0, '', "%d" % sid, command)
    if callback is not None:
      self.sysq[sid] = [sid, callback, frame, self.systimenext, self.systimes, transpr, command]
    return frame
  #
  def syscmd_done(self, sid, success, extra=''):
    if sid in self.sysq:
      cmd = self.sysq[sid]
      del self.sysq[sid]
      if type(cmd[1]) is bool:
        pass
      else:
        cmd[1](cmd[0], cmd[2], success, extra)
  #
  def frame_emit(self, frame):
    self.gate.frame_emit(self.addr, self.aux, frame)
  #
  def syscmd_emit(self, command, args=None, callback=None, transpr=None):
    frame = self.syscmd_make(command, args, callback, None, transpr)
    if transpr:
      pass
    else:
      if self.other_uid is None:
        return # can't emit this frame
    self.frame_emit(frame)
    return frame
  #
  def syscmd_emit_ack(self, sid, ext=None):
    frame = self.syscmd_make('ACK', ext, None, sid)
    self.frame_emit(frame)
  #
  def syscmd_emit_nak(self, sid, ext=None):
    frame = self.syscmd_make('NAK', ext, None, sid)
    self.frame_emit(frame)
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
    # process retries
    if tick>0:
      for k in self.sysq.keys():
        vv = self.sysq[k]
        #self.d_info("try retry syscmd %d (%g %d) by %g", k, vv[3], vv[4], tick)
        vv[3] = vv[3] - tick
        if vv[3]<0:
          vv[4] = vv[4] - 1
          vv[3] = self.systimenext
          if vv[4]<0:
            self.d_warning("timeout syscmd %d %s", k, repr(vv[6]))
            self.syscmd_done(k, None, None)
          else:
            self.frame_emit(vv[2])
            self.d_warning("retry syscmd %d %s", k, repr(vv[6]))
    #
    pass
    #
  #
  def exec_sysframe(self, sysflag, chkey, key, value):
    try:
      sc = self.SysCommand(self, chkey, key, value)
    except:
      self.d_error("Bad syscmd %s", repr((sysflag, chkey, key, value)))
      return
    #
    if sysflag:
      if sc.command=='ACK':
        self.syscmd_done(sc.args, True)
      elif sc.command=='NAK':
        self.d_warning("got NAK %s", sc.args)
        self.syscmd_done(sc.args, False)
      else:
        self.d_warning("Damaged syscmd %s", repr((sysflag, chkey, key, value)))
      return
    else:
      self.d_debug("syscmd{%s %s %s %s}", repr(sc.key), repr(sc.chkey), repr(sc.command), repr(sc.args))
      if self.on_syscmd(sc) is None:
        syscmd.nak(None)
    pass
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

