#!/usr/bin/python -B

import time
import sys
import random

from libnet.common import dbg, dbg_raw
from libnet.logic import *
from libnet.t_tcp import Gate_TCP, transport_tcp_tick
from libnet.s_echo import *

#-------------------------------------------------------------
#-------------------------------------------------------------

Debug = False
#Debug = True

if Debug:
  Gate_TCP.Client.d_clev(0, 1, 2, 3)
  Gate_TCP.d_clev(0, 1, 2, 3)
  #
  Node.d_clev(0, 1, 2, 3)
  Bus.d_clev(0, 1, 2, 3)
  Channel.d_clev(0, 1, 2, 3)
  #
  Service_Ping.d_clev(0, 1, 2, 3)
  Service_Echo.d_clev(0, 1, 2, 3)
else:
  Gate_TCP.Client.d_clev(0, 1, 2)
  Gate_TCP.d_clev(0, 1, 2)
  #
  Node.d_clev(0, 1, 2)
  Bus.d_clev(0, 1, 2)
  Channel.d_clev(0, 1, 2)
  #
  Service_Ping.d_clev(0, 1)
  Service_Echo.d_clev(0, 1, 2)


def loss_sim(ctx, node, gate, addr, frame):
  rnd = random.randint(0, ctx[1])
  if rnd < ctx[2]:
    return True
  return False

Node.loss_simulator = [loss_sim, 100, 70]
Node.loss_simulator = [loss_sim, 100, 10]
Bus.systimenext = 0.1
Bus.systimes = 10

#-------------------------------------------------------------
#-------------------------------------------------------------

tick = 0.01 # 10 ms
mtime = 0.0
node1 = Node('Node_1')
node2 = Node('Node_2')

Gate_TCP(node1, 'client', 'connect', 'localhost', 7000)
Gate_TCP(node2, 'server', 'listen', 'localhost', 7000)

Service_Echo(node2)

def Loop(num, fn=None):
  global mtime
  for i in range(1, num):
    transport_tcp_tick(tick)
    node1.on_tick(tick)
    node2.on_tick(tick)
    #
    if (fn is not None) and fn():
      sys.stderr.write("[%g]\n" % mtime)
      break
    #
    time.sleep(tick)
    mtime += tick
    sys.stderr.write('.')

def isConnected():
  return node1.bus_to(node2.uid) and node2.bus_to(node1.uid)

def gotsrv(sk, level, nid, sid):
#  print "***"
  print "[%g] *** scan:%s level:%s node:%s service:%s" % (mtime, sk, level, nid, sid)
#  print "***"
  ping = Service_Ping(node1)
#  ping.target(nid, sid, ('SEQ','RETRY'))
  ping.target(nid, sid, ('RETRY'))


Loop(400, isConnected)
node1.service_scan(1, ('echo',), gotsrv, 2)
Loop(50)
node1.service_scan(1, ('echo',), gotsrv, 2)


dbg_raw("")
