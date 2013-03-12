@echo off
call vcvars_vc9.bat
SET VC_PLATFORM=vs2008
echo make.bat -fMakefile.cygwin-vc %1 %2 %3 %4 %5
make.bat -fMakefile.cygwin-vc %1 %2 %3 %4 %5

