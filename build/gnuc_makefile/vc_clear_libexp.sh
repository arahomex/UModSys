#!/bin/sh

exp=${1%%.dll}.exp
lib=${1%%.dll}.lib
manifest=$1.manifest
dll=0

if [ -f $exp ] ; then
  echo rm $exp
  rm $exp
  dll=1
fi

if [ -f $lib ] ; then
  echo rm $lib
  rm $lib
  dll=1
fi

if [ -f $manifest ] ; then
  echo embed $manifest
  if [ $dll -eq 1 ] ; then
    resid="#2"
  else
    resid="#1"
  fi
  echo mt.exe -nologo "-manifest" "$manifest" "-outputresource:$1;$resid"
  mt.exe -nologo "-manifest" "$manifest" "-outputresource:$1;$resid"
  rm $manifest
fi
