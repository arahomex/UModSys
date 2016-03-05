#!/bin/sh

#cd ../gnuc_makefile
#make -fMakefile.linux $1 $2 $3 $4 $5

cd ../gnu-make
MAKEOPT=-j5 ./make.sh "$@"


