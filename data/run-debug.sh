#!/bin/sh

ME="`readlink -e \"$0\"`"
BP="`dirname \"$ME\"`"

cd "$BP"

PLATFORM=`uname`_`uname -p`
MODE='t'
MODE=''

export LD_LIBRARY_PATH="LD_LIBRARY_PATH:`readlink -e ../bin/$PLATFORM.Debug`"
export UMS_COLORS=1
export UMS_STDLOG="UModSys-Std.log"
../bin/$PLATFORM.Debug/umodsysc$MODE.$PLATFORM.Debug "$@"

echo "End"
echo -e "\e[0m"
