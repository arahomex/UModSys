#!/bin/bash

cd "`dirname \"$0\"`"

all=(
  cp-umodsys-umodsys_lib_common
  cp-umodsys-umodsys_lib_core
  cp-umodsys-umodsys_lib_kernel

  cp-umodsys-umodsysct
  cp-umodsys-umodsysc
  cp-umodsys-umodsysca

#  cp-umodsys-plugin_lib2d_stdlib
#  cp-umodsys-plugin_lib3d_stdlib
#  cp-umodsys-plugin_libMedia_imagesstd
#  cp-umodsys-plugin_libMedia_serializeJson
#  cp-umodsys-plugin_libMedia_serializeYaml
#  cp-umodsys-plugin_libMedia_stdio
#  cp-umodsys-plugin_libMedia_stdlib
#  cp-umodsys-plugin_libMedia_zlib
#  cp-umodsys-plugin_libStd
#  cp-umodsys-plugin_libUI
#  cp-umodsys-plugin_libUI_SDL
#  cp-umodsys-plugin_libUI_SDLGL
#  cp-umodsys-plugin_libUI_SDLcore
#  cp-umodsys-plugin_libUI_frames

#  cp-umodsys-plugin_test0
#  cp-umodsys-plugin_test1
#  cp-umodsys-plugin_test2
#  cp-umodsys-plugin_test3
#  cp-umodsys-plugin_test4
#  cp-umodsys-plugin_test5
#  cp-umodsys-plugin_test6
#  cp-umodsys-plugin_test7
)

exec ./make.sh -j5 "${all[@]}"


