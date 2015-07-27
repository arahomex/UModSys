#!/bin/sh

export PLAT=cmake-gcc.`uname`.`uname -p`
export UMS_PID=linux


function MakeTarget() {
  
  Target=$1
  if [ ! -d ../bin/$PLAT.$Target ] ; then
    echo "Not found dir: $PLAT.$Target"
    exit
  fi
  ../bin/$PLAT.$Target/umodsys
}

pushd ../../data

if [ "$1" == "clean" ] ; then
  maketarget=clean
  shift
fi

if [ "$1" == "" ] ; then
#  MakeTarget None
  MakeTarget Debug
#  MakeTarget Release
#  MakeTarget MinSizeRel
#  MakeTarget RelWithDebInfo
else
  while [ "$1" != "" ] ; do
    MakeTarget $1
    shift
  done
fi

popd
