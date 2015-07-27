@echo off
rem %1 is build mode
rem %2.. is list of version tags
if "%1"=="" exit /b
SET R=%~d0%~p0
SET M=%1

rem Loop for all version tags
:next
shift
if "%1"=="" exit /b
call %R%Versions\version-%1-%M%.bat
goto next
