#!/bin/bash

#
# Usage make-resources-unzip <target> file...
#
#

function upmove() {
  local pref="$1" fn fn1 fn2
  shift
#  true "$@"
  for fn in "$@" ; do
    fn1="`basename \"$fn\"`"
    fn2="$pref..${fn//\//.}"
    mv "$fn" "../$fn2"
  done
}

function extract() {
  local fn="$1"
  local ffn="`basename \"$fn\"`"
  local ufn="${ffn//\//.}"
  echo $ufn
  pushd "$target" >/dev/null
  [ -z "$ffn" ] && exit 3
  mkdir -p "$ffn"
  cd "$ffn"
  unzip "$fn"
  find * -type f -print0 | xargs -0 "$me" --upmove "$ffn"
  cd ..
  [ -z "$ffn" ] && exit 3
  rm -rf "$ffn"
  popd >/dev/null
}

me="`readlink -e \"$0\"`"
target="$1"
shift

if [ -z "$target" ] ; then
  echo "Usage: $me <target-dir> <src-zip-files>" >&2
elif [ "$target" == "--upmove" ] ; then
  echo "$@"
  upmove "$@"
else
  target="`readlink -e \"$target\"`"
  mkdir -p "$target"

  for fn in "$@" ; do
    extract "`readlink -e \"$fn\"`"
  done
fi

