
set(S ${SRC}/lib/3d)
set(SM ${SRC}/module/3d)

include_directories (${INC})

add_library(umodsys.lib3d STATIC
 ${S}/umodsys.lib.3d.common.cpp
 ${SM}/stdlib/umodsys.mod.3d.stdlib.cpp
)

