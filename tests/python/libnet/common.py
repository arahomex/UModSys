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

def length_pack(value):
  if value<0:
    raise Exception('Value need to be>=0')
  elif value>=0x100000000:
    raise Exception('Value need to be less 32 bit')
  elif value==0:
    return (0, '') 
  elif value<=0xff:
    return (1, struct.pack('!B', value))
  elif value<=0xffff:
    return (2, struct.pack('!H', value))
  else:
    return (3, struct.pack('!I', value))

def length_unpack(binary, idx, lid):
  if lid==0:
    return (idx,0)
  elif lid==1:
    val, = struct.unpack('!B', binary[idx:idx+1])
    return (idx+1, val)
  elif lid==2:
    val, = struct.unpack('!H', binary[idx:idx+2])
    return (idx+2, val)
  elif lid==3:
    val, = struct.unpack('!I', binary[idx,idx+4])
    return (idx+4, val)
  else:
    raise Exception('Value need to be [0..3]')


def make_frame(sysflag, chid, chkey, key, value):
  sysflag = 1 if sysflag else 0
  chkeylen = len(chkey)
  if chkeylen>0xff:
    raise Exception('Channel key too long')
  keylen = len(key)
  valuelen = len(value)
  l_chid, b_chid = length_pack(chid)
  l_key, b_key = length_pack(keylen)
  l_val, b_val = length_pack(valuelen)
  return struct.pack('!B', (sysflag<<7) | ((1<<6) if chkeylen else 0) | (l_chid<<4) | (l_key<<2) | l_val ) \
    + b_chid + b_key + b_val \
    + (struct.pack('!B', chkeylen)+chkey if chkeylen else '') \
    + key + value

def parse_frame(binary):
  if binary is None or (binary==''):
    return ('', None)
  try:
    inf, = struct.unpack('!B', binary[0:1] )
    sysflag = inf & (1<<7)
    l_chkey = inf & (1<<6)
    l_chid = (inf>>4) & 0x03
    l_key = (inf>>2) & 0x03
    l_val = inf & 0x03
    idx = 1
    idx, chid = length_unpack(binary, idx, l_chid)
    idx, keylen = length_unpack(binary, idx, l_key)
    idx, vallen = length_unpack(binary, idx, l_val)
    idx, chkeylen = length_unpack(binary, idx, 1 if l_chkey else 0)
    chkey = binary[idx:idx+chkeylen]
    idx = idx + chkeylen
    key = binary[idx:idx+keylen]
    idx = idx + keylen
    value = binary[idx:idx+vallen]
    idx = idx + vallen
    binary = binary[idx:]
  except:
    raise
  return (binary, (sysflag!=0, chid, chkey, key, value))


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

