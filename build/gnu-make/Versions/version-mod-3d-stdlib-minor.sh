#!/bin/sh
bp=`realpath $0`
bp=`dirname $bp`
cd $bp/../../..
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --mode:minor --name:mod-3d-stdlib --set:platform:platform/linux
