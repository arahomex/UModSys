@echo off
echo ************************ Generate SDL_GL opengl imports
pushd %~d0%~p0\..\..\src\module\ui\SDL_GL
perl ../../../../build/_gen/opengl.pl --funcs:opengl_genlist.txt --debug:3
popd
echo ************************ Done

