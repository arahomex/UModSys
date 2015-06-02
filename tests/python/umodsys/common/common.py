import time
import sys
import weakref
import struct
import zlib
import random
import string

#-------------------------------------------------------------
#-------------------------------------------------------------

DebugLevels = 0xffffffff
DebugFiles = [sys.stderr] * 32

def dbg(L, S):
  if (1<<L) & DebugLevels:
    print >>DebugFiles[L], S

def dbg_raw(L, S):
  print >>DebugFiles[L], S

#-------------------------------------------------------------
#-------------------------------------------------------------

class DbgMultiTarget(object):
  targets = None
  #
  def __init__(self, *targets):
    self.targets = targets
  #
  def write(self, data):
    for t in self.targets:
      t.write(data)
  #
  pass

#-------------------------------------------------------------
#-------------------------------------------------------------

def is_array(var):
    return isinstance(var, (list, tuple))

def next_word(line, key=' '):
  if line is None:
    return '', None
  if line=='':
    return '', None
  pos = line.find(key)
  if pos==-1:
    return line, None
  return (line[:pos], line[pos+1:])

def next_words(args, nargs, naux=0, cvt=None, key=' '):
  ret = []
  for i in range(0, nargs):
    val, args = next_word(args, key)
    if val=='':
      dbg(0, "Can't get arg %d" % (i))
      return (None, None)
    if (cvt is not None) and (cvt[i] is not None):
      try:
        val = cvt[i](val)
      except:
        dbg(0, "Can't convert %s by %s" % (repr(val), repr(cvt[i])))
        return (None, None)
    ret.append(val)
  for i in range(0, naux):
    val, args = next_word(args, key)
    if val=='':
      break
    ret.append(val)
  return (ret, args)

def as_integer(value):
  try:
    return int(value)
  except:
    return None


random_char_set = string.ascii_uppercase + string.ascii_lowercase + string.digits

def random_key(length):
  return ''.join(random.sample(random_char_set*length, length))


raise_for_num_map = {}
def raise_for_num(key, num):
  if key not in raise_for_num_map:
    raise_for_num_map[key] = num
  else:
    v = raise_for_num_map[key] - 1
    if v<=0:
      raise Exception("raise_for_num(%s, %d)" % (repr(key), num))
    raise_for_num_map[key] = v
  

#-------------------------------------------------------------

class BaseObject(object):
  #
  uid = None
  #
  DL_DEBUG = None
  DL_INFO = None
  DL_WARNING = None
  DL_ERROR = None
  #
  class __metaclass__(type):
    pass
  #
  @classmethod
  def d_clev(cls, e=None, w=None, i=None, d=None):
    cls.DL_DEBUG = d
    cls.DL_INFO = i
    cls.DL_WARNING = w
    cls.DL_ERROR = e
  #
  @classmethod
  def d_cgetid(cls):
    return "%.3f::%s::" % (time.time(), cls.__name__)
  #
  @classmethod
  def d_cinfo(cls, fmt, *args):
    if cls.DL_INFO is not None:
      if DebugLevels & (1<<cls.DL_INFO):
        dbg_raw(cls.DL_INFO, cls.d_cgetid() + "INFO " +  (fmt % args))
  #
  @classmethod
  def d_cwarning(cls, fmt, *args):
    if cls.DL_WARNING is not None:
      if DebugLevels & (1<<cls.DL_WARNING):
        dbg_raw(cls.DL_WARNING, cls.d_cgetid() +"WARNING " +  (fmt % args))
  #
  @classmethod
  def d_cerror(cls, fmt, *args):
    if cls.DL_ERROR is not None:
      if DebugLevels & (1<<cls.DL_ERROR):
        dbg_raw(cls.DL_ERROR, cls.d_cgetid() + "ERROR " + (fmt % args))
  #
  @classmethod
  def d_cdebug(cls, fmt, *args):
    if cls.DL_DEBUG is not None:
      if DebugLevels & (1<<cls.DL_DEBUG):
        dbg_raw(cls.DL_DEBUG, cls.d_cgetid() + "DEBUG " + (fmt % args))
  #
  #
  def d_getid(self):
    return "%.3f::%s(%s)::" % (time.time(), self.__class__.__name__, self.uid)
  #
  def d_info(self, fmt, *args):
    if self.DL_INFO is not None:
      if DebugLevels & (1<<self.DL_INFO):
        dbg_raw(self.DL_INFO, self.d_getid() + "INFO " + (fmt % args))
  #
  def d_warning(self, fmt, *args):
    if self.DL_WARNING is not None:
      if DebugLevels & (1<<self.DL_WARNING):
        dbg_raw(self.DL_WARNING, self.d_getid() + "WARNING " + (fmt % args))
  #
  def d_error(self, fmt, *args):
    if self.DL_ERROR is not None:
      if DebugLevels & (1<<self.DL_ERROR):
        dbg_raw(self.DL_ERROR, self.d_getid() + "ERROR " + (fmt % args))
  #
  def d_debug(self, fmt, *args):
    if self.DL_DEBUG is not None:
      if DebugLevels & (1<<self.DL_DEBUG):
        dbg_raw(self.DL_DEBUG, self.d_getid() + "DEBUG " + (fmt % args))
  #
  #
  #

#-------------------------------------------------------------
#-------------------------------------------------------------

