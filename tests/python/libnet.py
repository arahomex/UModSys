#!/usr/bin/python -B

import time
import sys

from libnet.transport import Gate_TCP, transport_tick
from libnet.logic import Node

#-------------------------------------------------------------
#-------------------------------------------------------------

tick = 0.5 # 10 ms
node1 = Node('Node_1')
node2 = Node('Node_2')

node1.gate_add( Gate_TCP('client', 'connect', 'localhost', 7000) )
node2.gate_add( Gate_TCP('server', 'listen', 'localhost', 7000) )

while True:
  transport_tick(tick)
  node1.tick(tick)
  node2.tick(tick)
  time.sleep(tick)
  sys.stderr.write('.')


