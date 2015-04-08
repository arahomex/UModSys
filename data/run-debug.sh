#!/bin/sh

PLATFORM=`uname`.`uname -p`
MODE='t'
#MODE=''

../bin/$PLATFORM.Debug/umodsysc$MODE.$PLATFORM.Debug

