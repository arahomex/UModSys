#!/bin/bash +x

./generate.sh || exit $?

P=`uname`_`uname -p`

if [ -n "$1" ] ; then
  C=$1
  shift
else
  C=Debug
fi

echo "make -f Makefile.$P.$C" "$@"
make -f Makefile.$P.$C "$@"


