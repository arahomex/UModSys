@echo off
call vcvars_vc8.bat
SET VC_PLATFORM=vs2005
echo make.bat -fMakefile.cygwin-vc %1 %2 %3 %4 %5
make.bat -fMakefile.cygwin-vc %1 %2 %3 %4 %5

