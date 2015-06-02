import time
import sys
import weakref
import struct
import zlib

from ..common.common import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Library(BaseObject):
  #
  nodes = None
  #
  def __init__(self):
    BaseObject.__init__(self)
    #
    self.nodes = {}
  #
  def point_filename(self, point, filename):
    if filename.startswith(point[1]):
      return filename[len(filename):]
    return None
  #
  def point_add(self, lev, path, arc):
    if not path.endswith('/'):
      path += '/'
    if not path.startswith('/'):
      path += '/'
    self.nodes[(lev, path)] = arc
  #
  def load(self, filename):
    for point,node in nodes.items():
      pix = self.point_filename(point, filename)
      if not pix:
        continue
      rv = node.load(pix)
      if rv is not None:
        return rv
    return None
  #
  def save(self, filename, data):
    for point,node in nodes.items():
      pix = self.point_filename(point, filename)
      if not pix:
        continue
      rv = node.save(pix, data)
      if rv:
        return rv
    return False
  #
  pass

#-------------------------------------------------------------
#-------------------------------------------------------------

