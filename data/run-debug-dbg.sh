#!/bin/sh

PLATFORM=`uname`.`uname -p`

pushd ../build/gnuc_makefile
./make-linux.sh DEBUG=1 || exit
popd

#gdb ../bin/$PLATFORM.Debug/umodsysc.$PLATFORM.Debug
valgrind --read-var-info=yes --track-origins=yes ../bin/$PLATFORM.Debug/umodsysc.$PLATFORM.Debug
