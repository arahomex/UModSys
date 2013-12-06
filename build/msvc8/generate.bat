@echo off
echo ************************ Generate build files...
pushd %~d0%~p0
perl ../_gen/gen_build.pl generator.txt
rem pp perl build/_gen/update_versions.pl update_versions.exe
rem bash -c "pp -o update_versions.exe ../_gen/update_versions.pl"
popd

echo ************************ Generate version files...
pushd %~d0%~p0..\..
if not exist %~d0%~p0\Versions\nul mkdir %~d0%~p0\Versions
del %~d0%~p0\Versions\*.bat
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --genpath:%~d0%~p0/Versions --gentype:build/_gen/version-generator-bat.txt --set:platform:platform/win32
perl build/_gen/update_versions.pl --file:build/_gen/versions.txt --set:platform:platform/win32 --mode:auto --all
popd

echo ************************ Done

