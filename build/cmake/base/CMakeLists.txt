
set(S ${SRC}/base)
set(SP ${SRC}/base/${PLATSRC})

include_directories (${INC})

add_library(umodsys.base STATIC
 ${S}/umodsys.base.debug.cpp
 ${S}/umodsys.base.console.cpp
 ${S}/umodsys.base.system.cpp
 ${S}/umodsys.base.module.cpp
 ${S}/umodsys.base.moduleloader.cpp
 ${S}/umodsys.base.modulecontext.cpp
 ${S}/umodsys.base.moduleinfo.cpp
 ${S}/umodsys.base.modulereg.cpp
 ${S}/umodsys.base.moduleregchain.cpp
 ${SP}/umodsys.so_main.${PID}.cpp
)

