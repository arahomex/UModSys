#!/bin/sh

ME="`readlink -e \"$0\"`"
BP="`dirname \"$ME\"`"

cd "$BP"

PLATFORM=`uname`_`uname -p`
MODE='t'
MODE=''

export LD_LIBRARY_PATH="LD_LIBRARY_PATH:`readlink -e ../bin/$PLATFORM.Debug`"
../bin/$PLATFORM.Debug/umodsysc$MODE.$PLATFORM.Debug "$@"

