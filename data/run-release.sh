#!/bin/sh

PLATFORM=`uname`_`uname -p`
MODE='t'
#MODE=''

export LD_LIBRARY_PATH="LD_LIBRARY_PATH:`readlink -e ../bin/$PLATFORM.Release`"

../bin/$PLATFORM.Release/umodsysc$MODE.$PLATFORM.Release

