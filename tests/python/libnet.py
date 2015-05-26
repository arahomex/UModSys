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

#Debug = False
Debug = True
debugfile = open("debug.log", "w")

DebugFiles[8] = debugfile
DebugFiles[9] = debugfile
DebugFiles[10] = debugfile
DebugFiles[11] = debugfile

if Debug:
  Gate_TCP.Client.d_clev(8, 9, 10, 11)
  Gate_TCP.d_clev(8, 9, 10, 11)
  #
  Node.d_clev(8, 9, 10, 11)
  Bus.d_clev(8, 9, 10, 11)
  Channel.d_clev(8, 9, 10, 11)
  #
  Service_Ping.d_clev(8, 9, 10, 11)
  Service_Echo.d_clev(8, 9, 10, 11)
else:
  Gate_TCP.Client.d_clev(8, 9, 10)
  Gate_TCP.d_clev(8, 9, 10)
  #
  Node.d_clev(8, 9, 10)
  Bus.d_clev(8, 9, 10)
  Channel.d_clev(8, 9, 10)
  #
  Service_Ping.d_clev(8, 9)
  Service_Echo.d_clev(8, 9, 10)


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

print >>debugfile, ""
print ""

