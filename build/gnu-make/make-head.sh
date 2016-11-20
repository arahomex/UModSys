#!/bin/bash +ex

cd "`dirname \"$0\"`"

#MAKEOPTS=${MAKEOPTS:--j4}

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
#echo "'L=$L'"

if [ -z "$L" ] ; then
  L=Debug
elif [ "$L" == " all" ] ; then
  L="Debug Release ReleaseSpace"
fi

for C in $L ; do
  echo "make $MAKEOPTS -f Makefile.$P.$C" "$@"
  make $MAKEOPTS -f Makefile.$P.$C "$@" 2>&1 | head -n 50
  err=${PIPESTATUS[0]}
  (( $err )) || exit $err
done



