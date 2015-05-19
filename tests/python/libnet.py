#!/usr/bin/python -B

import time
import sys

from libnet.common import dbg, dbg_raw
from libnet.logic import *
from libnet.t_tcp import Gate_TCP, transport_tcp_tick
from libnet.s_echo import *

#-------------------------------------------------------------
#-------------------------------------------------------------

Debug = False
Debug = True

if Debug:
  Gate_TCP.Client.d_clev(0, 1, 2, 3)
  Gate_TCP.d_clev(0, 1, 2, 3)
  #
  Bus.d_clev(0, 1, 2, 3)
  Node.d_clev(0, 1, 2, 3)
  Channel.d_clev(0, 1, 2, 3)
  #
  Service_Ping.d_clev(0, 1, 2, 3)
  Service_Echo.d_clev(0, 1, 2, 3)
else:
  Gate_TCP.Client.d_clev(0, 1, 2)
  Gate_TCP.d_clev(0, 1, 2)
  #
  Bus.d_clev(0, 1, 2)
  Node.d_clev(0, 1, 2)
  Channel.d_clev(0, 1, 2)
  #
  Service_Ping.d_clev(0, 1, 2)
  Service_Echo.d_clev(0, 1, 2)

#-------------------------------------------------------------
#-------------------------------------------------------------

tick = 0.1 # 10 ms
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
#  print "***"
  print "*** scan:%s level:%s node:%s service:%s" % (sk, level, nid, sid)
#  print "***"
  ping = Service_Ping(node1)
  ping.target(nid, sid)


Loop(10)
node1.service_scan(1, ('echo',), gotsrv, 2)
Loop(20)
node1.service_scan(1, ('echo',), gotsrv, 2)


dbg_raw("")
