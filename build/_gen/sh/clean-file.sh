#!/bin/bash -e

args=()

for arg in "$@" ; do
  if [ -f "$arg" ] ; then
    args+=("$arg")
    rm "$arg"
  elif [ -d "$arg" ] ; then
    args+=("$arg")
    rm -r "$arg"
  fi
done
if [ ${#args[@]} -gt 0 ] ; then
  echo "rm ${args[@]}"
fi
