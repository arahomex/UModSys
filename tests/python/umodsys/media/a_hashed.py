import time
import sys
import weakref
import fnmatch
import io
import os

from ..common.common import *

from l_archive import *


#-------------------------------------------------------------
#-------------------------------------------------------------

class Archive_Hashed(Archive):
  #
  lib = None
  pwd = None
  files = None
  hashes = None
  #
  def __init__(self, lib, pwd, numtop=4):
    Archive.__init__(self)
    #
    self.lib = weakref.ref(lib)
    self.numtop = numtop
    self.pwd = lib.point_normdir(pwd)
    #
    self._info_load()
  #
  def _info_load(self):
    lst = self.lib().load(self.pwd+'files')
    self.d_debug("Load MD5 records: %s", repr(lst))
    files = SortedDict()
    hashes = {}
    if lst is not None:
      for line in lst.split("\n"):
        self.d_debug("Read MD5 record: %s", repr(line))
        inf = SortedDict()
        for opt in line.split("\t"):
          self.d_debug("Read MD5 record elem: %s", opt)
          k,v = next_word(opt, ':')
          inf[k] = v
        if 'filename' not in inf:
          continue
        if 'md5' not in inf:
          continue
        files[inf['filename']] = inf
        hashes[inf['md5']] = True
        self.d_debug("Found MD5 record: %s", repr(inf))
    self.files = files
    self.hashes = hashes
  #
  def _info_save(self):
    lines = []
    for inf in self.files.values():
      line = []
      for k,v in inf.items():
        line.append("%s:%s" % (k,v))
      lines.append("\t".join(line))
    return self.lib().save(self.pwd+'files', "\n".join(lines))
  #
  def _hash_fn(self, h):
    dr = ''
    if self.numtop>0:
      dr = h[0:self.numtop]
      return self.pwd+dr+"/"+h+".binary"
    return self.pwd+h
  #
  def _load(self, filename):
    if filename not in self.files:
      return None
    return self.lib().load( self._hash_fn(self.files[filename]['md5']) )
  #
  def _save(self, filename, data):
    md5 = self._calc_md5(data)
    if md5 not in self.hashes:
      if not self.lib().save(self._hash_fn(md5), data):
        return False
      self.hashes[md5] = True
    #
    self.files[filename] = SortedDict( filename= filename, md5= md5 )
    self._info_save()
    #
    return True
  #
  def hash_md5(self, filename):
    if filename not in self.files:
      return Archive.hash_md5(self, filename)
    return self.files[filename]['md5']
  #
  def list(self, filename):
    rv = []
    for fn in self.files.keys():
      if fnmatch.fnmatch(fn, filename):
        rv.append(fn)
    return rv
  #
  pass


#-------------------------------------------------------------
#-------------------------------------------------------------

