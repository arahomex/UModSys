@echo off
pushd %~d0%~p0..\..
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --mode:major --name:core --set::platform:platform/win32
popd %~d0%~p0..\..
