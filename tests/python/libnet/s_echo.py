import time
import sys
import weakref
import struct
import zlib

from com import *
from logic import *

#-------------------------------------------------------------
#-------------------------------------------------------------

class Service_Echo(Service):
  #
  uid = None
  node = None
  #
  def __init__(self, node):
    Service.__init__(self, "echo:%s" % random_key(4), node)
    #
  #
  def on_scan(self, kw):
    if kw=='echo':
      return True
    return False
  #
  pass

#-------------------------------------------------------------
#-------------------------------------------------------------

