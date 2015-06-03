import time
import sys
import weakref
import md5

from ..common.common import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Archive(BaseObject):
  #
  is_read = True
  is_write = True
  #
  def __init__(self):
    BaseObject.__init__(self)
  #
  def load(self, filename):
    if self.is_read:
      return self._load(filename)
    return None
  #
  def save(self, filename, data):
    if self.is_write:
      return self._save(filename, data)
    return None
  #
  def list(self, filename):
    return ()
  #
  def hash_md5(self, filename):
    data = self.load(filename)
    if data is None:
      return None
    return self._calc_md5(data)
  #
  #
  def _load(self, filename):
    return None
  #
  def _save(self, filename, data):
    return False
  #
  def _calc_md5(self, data):
    m = md5.new()
    m.update(data)
    return m.hexdigest()
  #
  pass


#-------------------------------------------------------------
#-------------------------------------------------------------

