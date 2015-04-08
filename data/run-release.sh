#!/bin/sh

PLATFORM=`uname`.`uname -p`
MODE='t'
MODE=''

../bin/$PLATFORM.Release/umodsysc$MODE.$PLATFORM.Release

