import time
import sys
import weakref
import struct
import zlib
import random
import string

#-------------------------------------------------------------
#-------------------------------------------------------------

DebugLevel = 8

def dbg(L, S):
  if L<DebugLevel:
    print >>sys.stderr, S

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

def next_word(line):
  if line is None:
    return '', None
  if line=='':
    return '', None
  pos = line.find(" ")
  if pos==-1:
    return line, None
  return (line[:pos], line[pos+1:])

random_char_set = string.ascii_uppercase + string.ascii_lowercase + string.digits

def random_key(length):
  return ''.join(random.sample(random_char_set*length, length))

#-------------------------------------------------------------
#-------------------------------------------------------------

