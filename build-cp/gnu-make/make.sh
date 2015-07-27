#!/bin/bash +x

cd "`dirname \"$0\"`"

if [ "$1" == "-" ] ; then
  shift
else
  ./generate.sh || exit $?
fi

P=`uname`_`uname -p`

L=""

while [[ $1 =~ @(.*) ]] ; do
  L="$L ${BASH_REMATCH[1]}"
  shift
done

if [ -z "$L" ] ; then
 L=Debug
fi

for C in $L ; do
  echo "make -f Makefile.$P.$C" "$@"
  make -f Makefile.$P.$C "$@" || exit $?
done



