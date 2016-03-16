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

if [ "$*" == "" ] ; then
#  DEFARGS="--dflag=+rsdl_SysTests --dflag=+rsdl_MemoryError --dflag=+rsdl_Uid --dflag=+rsdl_System --dflag=+rsdl_Module --dflag=+rsdl_ModuleLibrary --dflag=+rsdl_SoLoad --dflag=+rsdl_TCL"
  DEFARGS="--dflag=+rsdl_SysTests --dflag=+rsdl_MemoryError --dflag=-rsdl_Uid --dflag=-rsdl_System --dflag=-rsdl_Module --dflag=-rsdl_ModuleLibrary --dflag=-rsdl_SoLoad --dflag=+rsdl_TCL"
  DEFARGS="$DEFARGS --script=debug.umstcl"
fi

echo +../bin/$PLATFORM.Debug/umodsysc$MODE.$PLATFORM.Debug $DEFARGS "$@"
../bin/$PLATFORM.Debug/umodsysc$MODE.$PLATFORM.Debug $DEFARGS "$@"

echo "End"
echo -e "\e[0m"
