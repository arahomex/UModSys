#!/bin/sh

export PLAT=cmake-gcc.`uname`.`uname -p`
export UMS_PID=linux

function MakeTarget() {
  Target=$1
  if [ ! -d $PLAT.$Target ] ; then
    mkdir $PLAT.$Target || exit
  fi
  cd $PLAT.$Target || exit
  export UMS_PLAT=$PLAT.$Target
  cmake -G "Unix Makefiles" ../../build/cmake/ -DCMAKE_BUILD_TYPE=$Target || exit
  make $maketarget || exit
  cd ..
}

pushd ../../tmp

if [ "$1" == "clean" ] ; then
  maketarget=clean
  shift
fi

if [ "$1" == "" ] ; then
  MakeTarget None
  MakeTarget Debug
  MakeTarget Release
  MakeTarget MinSizeRel
  MakeTarget RelWithDebInfo
else
  while [ "$1" != "" ] ; do
    MakeTarget $1
    shift
  done
fi

popd
