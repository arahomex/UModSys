@echo off
pushd %~d0%~p0..\..\..
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --mode:minor --name:mod-2d-stdlib --set::platform:platform/win32
popd
