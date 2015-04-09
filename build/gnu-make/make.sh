#!/bin/bash -x

P=`uname`_`uname -p`

make -f Makefile target-$P-Debug

