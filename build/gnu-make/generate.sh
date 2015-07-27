#!/bin/bash

bp=`readlink -e $0`
bp=`dirname $bp`

cd $bp
echo '************************ Generate build files...'
export PLATFORM="`uname`_`uname -p`"
export PLATFORMS="`uname`_`uname -p`"
perl ../_generator/gen_build.pl generator.txt
exit
echo '************************ Generate version files...'
cd $bp/../..
if [ ! -d $bp/Versions ] ; then
  mkdir $bp/Versions
  rm $bp/Versions/*.sh
fi
perl build/_generator/update_versions.pl --file:build/_gen_rules/versions.txt --genpath:$bp/Versions --gentype:build/_gen/version-generator-sh.txt --set:platform:platform/linux
perl build/_generator/update_versions.pl --file:build/_gen_rules/versions.txt --set:platform:platform/linux --mode:auto --all

echo '************************ Done'

