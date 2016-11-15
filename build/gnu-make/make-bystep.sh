#!/bin/bash

cd "`dirname \"$0\"`"

all=(
  clean

  tp-umodsys-umodsys_lib_common
  tp-umodsys-umodsys_lib_core
  tp-umodsys-umodsys_lib_kernel

  tp-umodsys-umodsysc
#  tp-umodsys-umodsysca
  tp-umodsys-umodsysct

)

exec ./make.sh "${all[@]}"


