#!/usr/bin/python -B

import time
import sys
import random

from libnet.common import dbg, dbg_raw, DbgMultiTarget
from libnet.logic import *
from libnet.t_tcp import Gate_TCP, transport_tcp_tick
from libnet.s_echo import *

#-------------------------------------------------------------
#-------------------------------------------------------------

debugfile = open("debug.log", "w")
debugfile2 = DbgMultiTarget(debugfile, sys.stderr)

for i in range(0,7): DebugFiles[i] = debugfile2
for i in range(8,31): DebugFiles[i] = debugfile

Gate_TCP.Client.d_clev(4, 5, 10, 11)
Gate_TCP.d_clev(4, 5, 10, 11)
#
Node.d_clev(4, 5, 10, 11)
Bus.d_clev(4, 5, 10, 11)
#
MetaChannel.d_clev(4, 5, 10, 11)
MetaChannel.Simple.d_clev(4, 5, 10, 11)
MetaChannel.Ordered.d_clev(4, 5, 10, 11)
MetaChannel.Retry.d_clev(4, 5, 10, 11)
MetaChannel.Stream.d_clev(4, 5, 10, 11)
#
Service_Ping.d_clev(4, 5, 6, 15)
Service_Echo.d_clev(4, 5, 6, 15)


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

ping = Service_Ping(node1)

def gotsrv(sk, level, nid, sid):
  dbg_raw(0, "[%g] *** scan:%s level:%s node:%s service:%s" % (mtime, sk, level, nid, sid))
#  ping.target(nid, sid, ())
#  ping.target(nid, sid, ('ORDERED'))
#  ping.target(nid, sid, ('RETRY'))
  ping.target(nid, sid, ('STREAM'))


Loop(400, isConnected)
node1.service_scan(1, ('echo',), gotsrv, 2)
Loop(400)
node1.service_scan(1, ('echo',), gotsrv, 2)

dbg_raw(8, "")
sys.stderr.write("\n")
print "Statistics: " + ping.statistics() + "\n"
