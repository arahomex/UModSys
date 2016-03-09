#!/bin/bash

bp=`readlink -e $0`
bp=`dirname $bp`

cd $bp
echo '************************ Generate build files...'
export PLATFORM="`uname`_`uname -p`"
export PLATFORMS="`uname`_`uname -p`"
perl ../_gen/gen_build.pl  --verbose:0 generator.txt
#exit
echo '************************ Generate version files...'
cd $bp/../..
if [ ! -d $bp/Versions ] ; then
  mkdir $bp/Versions
else
  find $bp/Versions -name '*.sh' | xargs rm
fi
perl build/_gen/update_versions.pl --debug:0 --file:build/_gen_rules/versions.txt --genpath:$bp/Versions --gentype:build/_gen_rules/version-generator-sh.txt --set:platform:platform/linux
perl build/_gen/update_versions.pl --debug:0 --file:build/_gen_rules/versions.txt --set:platform:platform/linux --mode:auto --all

echo '************************ Done'

