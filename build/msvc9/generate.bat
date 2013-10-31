@echo off
pushd %~d0%~p0
rem perl ../_gen/gen_build.pl generator.txt
popd
pushd %~d0%~p0..\..
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --genpath:%~d0%~p0 --gentype:build/_gen/version-generator-bat.txt --set:platform:platform/win32
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --set:platform:platform/win32 --mode:auto --all
popd
