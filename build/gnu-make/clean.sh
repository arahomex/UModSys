#!/bin/bash -x

P=`uname`_`uname -p`

make -f Makefile clean-$P-Debug "$@"

