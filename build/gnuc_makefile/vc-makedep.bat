@echo off
ECHO %2: \
FOR /F "tokens=1,2,3,*" %%A IN ('%1 /nologo /c %2 /Zs /showIncludes') DO IF NOT "%%D"=="" echo %%D \