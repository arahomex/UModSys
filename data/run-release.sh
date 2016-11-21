#!/bin/sh

ME="`readlink -e \"$0\"`"
BP="`dirname \"$ME\"`"

cd "$BP"


PLATFORM=`uname`_`uname -p`
MODE='t'
#MODE=''

export LD_LIBRARY_PATH="LD_LIBRARY_PATH:`readlink -e ../bin/$PLATFORM.Release`"

../bin/$PLATFORM.Release/umodsysc$MODE.$PLATFORM.Release "$@"

