#!/bin/bash

cd "`dirname \"$0\"`"

all=(
  clean

  tp-umodsys-umodsys_lib_common
  tp-umodsys-umodsys_lib_core
  tp-umodsys-umodsys_lib_kernel

  tp-umodsys-umodsysct
  tp-umodsys-umodsysc
  tp-umodsys-umodsysca

  tp-umodsys-plugin_lib2d_stdlib
  tp-umodsys-plugin_lib3d_stdlib
  tp-umodsys-plugin_libMedia_imagesstd
  tp-umodsys-plugin_libMedia_serializeJson
  tp-umodsys-plugin_libMedia_serializeYaml
  tp-umodsys-plugin_libMedia_stdio
  tp-umodsys-plugin_libMedia_stdlib
  tp-umodsys-plugin_libMedia_zlib
  tp-umodsys-plugin_libStd
  tp-umodsys-plugin_libUI
  tp-umodsys-plugin_libUI_SDL
  tp-umodsys-plugin_libUI_SDLGL
  tp-umodsys-plugin_libUI_SDLcore
  tp-umodsys-plugin_libUI_frames

  tp-umodsys-plugin_test0
  tp-umodsys-plugin_test1
  tp-umodsys-plugin_test2
  tp-umodsys-plugin_test3
  tp-umodsys-plugin_test4
  tp-umodsys-plugin_test5
  tp-umodsys-plugin_test6
  tp-umodsys-plugin_test7

)

exec ./make.sh -j5 "${all[@]}"


