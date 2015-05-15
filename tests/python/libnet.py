#!/usr/bin/python -B

import time
import sys

from libnet.logic import Node
from libnet.t_tcp import Gate_TCP, transport_tcp_tick
from libnet.s_echo import *

#-------------------------------------------------------------
#-------------------------------------------------------------

tick = 0.2 # 10 ms
node1 = Node('Node_1')
node2 = Node('Node_2')

Gate_TCP(node1, 'client', 'connect', 'localhost', 7000)
Gate_TCP(node2, 'server', 'listen', 'localhost', 7000)

Service_Echo(node2)

def Loop(num):
  for i in range(1, num):
    transport_tcp_tick(tick)
    node1.tick(tick)
    node2.tick(tick)
    time.sleep(tick)
    sys.stderr.write('.')

def gotsrv(sk, level, nid, sid):
  print "scan:%s level:%s node:%s service:%s" % (sk, level, nid, sid)

Loop(10)
node1.service_scan(1, ('echo',), gotsrv, 2)
Loop(20)


