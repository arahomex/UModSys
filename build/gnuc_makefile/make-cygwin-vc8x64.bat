@echo off
call vcvars_vc8.bat x64
SET VC_PLATFORM=vs2005x64
echo make.bat -fMakefile.cygwin-vc %1 %2 %3 %4 %5
make.bat -fMakefile.cygwin-vc %1 %2 %3 %4 %5

