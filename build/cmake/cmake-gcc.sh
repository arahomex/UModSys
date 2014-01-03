#!/bin/sh

PLAT=cmake-gcc.`uname`.`uname -p`
export UMS_PID=linux
#R=`realpath $0`
#R=`dirname $R`
#echo $R
#exit

pushd ../../tmp

if [ ! -d $PLAT.Release ] ; then
  mkdir $PLAT.Release
fi
if [ ! -d $PLAT.Debug ] ; then
  mkdir $PLAT.Debug
fi

cd $PLAT.Debug
export UMS_PLAT=$PLAT.Debug
cmake -G "Unix Makefiles" ../../build/cmake/ -DCMAKE_BUILD_TYPE=Debug || exit
make || exit
cd ..

cd $PLAT.Release
export UMS_PLAT=$PLAT.Release
cmake -G "Unix Makefiles" ../../build/cmake/ -DCMAKE_BUILD_TYPE=Release || exit
make || exit
cd ..

cd $PLAT.Release
export UMS_PLAT=$PLAT.MinSizeRel
cmake -G "Unix Makefiles" ../../build/cmake/ -DCMAKE_BUILD_TYPE=MinSizeRel || exit
make || exit
cd ..


popd
