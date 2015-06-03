import time
import sys
import os
import weakref
import struct
import glob

from ..common.common import *

from l_archive import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Archive_Sys(Archive):
  #
  path = None
  #
  def __init__(self, path, ro=True):
    Archive.__init__(self)
    if not path.endswith('/'):
      path += '/'
    self.path = path
    self.is_write = not ro
  #
  def _load(self, filename):
    fn = self.path + filename
    try:
      with open(fn, 'r') as f:
        return f.read()
    except Exception, ee:
      self.d_warning("Can't read '%s': %s", fn, repr(ee))
      return None
  #
  def _save(self, filename, data):
    fn = self.path + filename
    self.d_debug("Write '%s'", fn)
    try:
      if not mkdir_file(fn):
        self.d_warning("Can't create directory for file '%s': %s", fn, repr(ee))
        return False
      with open(fn, 'w') as f:
        f.write(data)
    except Exception, ee:
      self.d_warning("Can't write '%s': %s", fn, repr(ee))
      return False
    return True
  #
  def list(self, filename):
    rv = []
    plen = len(self.path)
    for fn in glob.glob(self.path+filename):
      rv.append(fn[plen:])
    return rv
  #
  pass


#-------------------------------------------------------------
#-------------------------------------------------------------

