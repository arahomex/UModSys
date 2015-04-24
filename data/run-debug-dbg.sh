#!/bin/sh

Binary=''

function make_gm() {
  PLATFORM=`uname`.`uname -p`
  MODE='t'
  MODE=''
  
  pushd ../build/gnuc_makefile
  ./make-linux.sh DEBUG=1 || exit
  popd
  
  Binary=../bin/$PLATFORM.Debug/umodsysc$MODE.$PLATFORM.Debug
}

function make_genm() {
  PLATFORM=`uname`_`uname -p`

  pushd ../build/gnu-make
  ./make.sh Debug || exit
  popd

  Binary=../bin/$PLATFORM.Debug/umodsysc$MODE.$PLATFORM.Debug
}

function vg() {
  valgrind -v \
    --leak-check=full \
    --show-leak-kinds=all \
    --read-var-info=yes \
    --track-origins=yes \
    $Binary \
  2>&1 \
  | tee valgrind.log
}

function dbg() {
  echo "gdb $Binary"
  gdb $Binary
}

make_genm

export LD_LIBRARY_PATH="LD_LIBRARY_PATH:`readlink -e $(dirname $Binary)`"

dbg
