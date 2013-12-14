#!/bin/sh
bp=`realpath $0`
bp=`dirname $bp`
cd $bp/../../..
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --mode:major --name:mod-2d-stdlib --set:platform:platform/linux
