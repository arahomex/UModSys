@echo off
pushd %~d0%~p0..\..
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --set:platform:platform/win32 %1 %2 %3 
popd
