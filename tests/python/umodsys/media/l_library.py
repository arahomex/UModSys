import time
import sys
import weakref

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
    self.nodes = SortedDict()
  #
  def point_filename(self, point, filename):
    path = point[1]
    if filename.startswith(path):
      return filename[len(path):]
    return None
  #
  def point_norm(self, path):
    if not path.startswith('/'):
      path = '/' + path
    return path
  #
  def point_normdir(self, path):
    if not path.startswith('/'):
      path = '/' + path
    if not path.endswith('/'):
      path += '/'
    return path
  #
  def point_add(self, lev, path, arc):
    path = self.point_normdir(path)
    self.nodes[(lev, path)] = arc
  #
  def load(self, filename):
    filename = self.point_norm(filename)
    for point,node in self.nodes.items():
      pix = self.point_filename(point, filename)
      self.d_debug('Load "%s" pix %s %s', filename, repr((point,node)), repr(pix))
      if not pix:
        continue
      rv = node.load(pix)
      if rv is not None:
        return rv
    self.d_warning('Load "%s" failed', filename)
    return None
  #
  def save(self, filename, data):
    filename = self.point_norm(filename)
    for point,node in self.nodes.items():
      pix = self.point_filename(point, filename)
      self.d_debug('Save "%s" pix %s %s', filename, repr((point,node)), repr(pix))
      if not pix:
        continue
      rv = node.save(pix, data)
      if rv:
        return rv
    return False
  #
  def list(self, filename):
    filename = self.point_norm(filename)
    self.d_debug('List files %s', filename)
    ret = []
    for point,node in self.nodes.items():
      pix = self.point_filename(point, filename)
      self.d_debug('List pix %s %s', repr((point,node)), repr(pix))
      if not pix:
        continue
      rv = node.list(pix)
      if rv:
        for fn in rv:
          ret.append(point[1]+fn)
    return ret
  #
  def hash_md5(self, filename):
    filename = self.point_norm(filename)
    for point,node in self.nodes.items():
      pix = self.point_filename(point, filename)
      self.d_debug('Hash-md5 "%s" pix %s %s', filename, repr((point,node)), repr(pix))
      if not pix:
        continue
      rv = node.hash_md5(pix)
      if rv:
        return rv
    return None
  #
  #
  pass

#-------------------------------------------------------------
#-------------------------------------------------------------

