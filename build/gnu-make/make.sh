#!/bin/bash -x

./generate.sh || exit $?

P=`uname`_`uname -p`

if [ -n "$1" ] ; then
  C=$1
  shift
else
  C=Debug
fi

make -f Makefile.$P.$C "$@"


