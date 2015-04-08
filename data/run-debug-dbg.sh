#!/bin/sh

PLATFORM=`uname`.`uname -p`
MODE='t'
#MODE=''

pushd ../build/gnuc_makefile
./make-linux.sh DEBUG=1 || exit
popd

#gdb ../bin/$PLATFORM.Debug/umodsysc.$PLATFORM.Debug
valgrind -v --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes ../bin/$PLATFORM.Debug/umodsysc$MODE.$PLATFORM.Debug 2>&1 | tee valgrind.log
