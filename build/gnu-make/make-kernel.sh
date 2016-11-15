#!/bin/bash

cd "`dirname \"$0\"`"

exec ./make.sh \
 clean \
 tp-umodsys-umodsys_lib_common \
# \
 tp-umodsys-umodsys_lib_core \
 tp-umodsys-umodsys_lib_kernel


