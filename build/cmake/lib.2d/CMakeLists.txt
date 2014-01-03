
set(S ${SRC}/lib/2d)
set(SM ${SRC}/module/2d)

include_directories (${INC})

add_library(umodsys.lib2d STATIC
 ${S}/umodsys.lib.2d.common.cpp
 ${SM}/stdlib/umodsys.mod.2d.stdlib.cpp
)

