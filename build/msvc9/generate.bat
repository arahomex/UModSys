@echo off
pushd %~d0%~p0
perl ../_gen/gen_build.pl generator.txt
popd
