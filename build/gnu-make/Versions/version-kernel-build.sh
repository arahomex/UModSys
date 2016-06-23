#!/bin/sh
bp=`realpath $0`
bp=`dirname $bp`
cd $bp/../../..
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --mode:build --name:kernel --set:platform:platform/linux
