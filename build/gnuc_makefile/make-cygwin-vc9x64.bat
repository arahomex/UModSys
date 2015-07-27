@echo off
call vcvars_vc9.bat x64
SET VC_PLATFORM=vs2008x64
echo make.bat -fMakefile.cygwin-vc %1 %2 %3 %4 %5
make.bat -fMakefile.cygwin-vc %1 %2 %3 %4 %5

