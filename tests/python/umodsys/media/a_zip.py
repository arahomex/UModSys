import time
import sys
import weakref
import zipfile
import fnmatch
import io

from ..common.common import *

from l_archive import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Archive_Zip(Archive):
  #
  zipfile = None
  #
  def __init__(self, data):
    Archive.__init__(self)
    self.zipfile = zipfile.ZipFile(io.BytesIO(data))
    self.is_write = False
  #
  def _load(self, filename):
    try:
      return self.zipfile.read(filename)
    except:
      return None
  #
  def list(self, filename):
    rv = []
    for fn in self.zipfile.namelist():
      if fnmatch.fnmatch(fn, filename):
        rv.append(fn)
    self.d_debug("List '%s' %d files", filename, len(rv))
    return rv
  #
  pass


#-------------------------------------------------------------
#-------------------------------------------------------------

