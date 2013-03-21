#!/bin/sh

exp=${1%%.dll}.exp
lib=${1%%.dll}.lib
mainifest=$1.manifest

if [ -f $exp ] ; then
  echo rm $exp
  rm $exp
fi

if [ -f $lib ] ; then
  echo rm $lib
  rm $lib
fi

#if [ -f $mainifest ] ; then
#  echo rm $mainifest
#  rm $mainifest
#fi
