
set(S ${SRC}/basesys)
set(SP ${SRC}/basesys/${PLATSRC})

include_directories (${INC})

add_library(umodsys.basesys STATIC
 ${S}/umodsys.basesys.rsystem.cpp
 ${S}/umodsys.basesys.rsystem_main.cpp
 ${S}/umodsys.basesys.rsystem_tests.cpp
 ${S}/umodsys.basesys.rsystem_exec.cpp
 ${S}/umodsys.basesys.rmodule.cpp
 ${S}/umodsys.basesys.rmodulelib.cpp
 ${S}/umodsys.basesys.rmodulelibso.cpp
 ${S}/umodsys.basesys.rmodulelibthis.cpp
 ${S}/umodsys.basesys.rmoduleloader.cpp
 ${S}/umodsys.basesys.rparameters.cpp
 ${S}/umodsys.basesys.rconsole_std.cpp
 ${SP}/umodsys.basesys.rmodule.${PID}.cpp
)

add_executable(umodsys
 ${S}/umodsys.c_main.cpp
)
target_link_libraries(umodsys
  umodsys.basesys
  umodsys.libmedia
  umodsys.lib2d
  umodsys.base
  umodsys.core
  dl
)
