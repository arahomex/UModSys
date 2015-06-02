import time
import sys
import weakref
import struct
import zlib

from ..common.common import *

from l_archive import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Archive_Sys(Archive):
  #
  path = None
  ro = None
  #
  def __init__(self, path, ro=True):
    Archive.__init__(self)
    if not path.endswith('/'):
      path += '/'
    self.path = path
    self.ro = ro
  #
  def load(self, filename):
    try:
      with open(self.path + filename, 'r') as f:
        return f.read()
    except:
      return None
  #
  def save(self, filename, data):
    if not ro:
      return False
    try:
      with open(self.path + filename, 'w') as f:
        f.write(data)
    except:
      return False
    return True
  #
  pass


#-------------------------------------------------------------
#-------------------------------------------------------------

