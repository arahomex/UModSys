
set(S ${SRC}/lib/media)
set(SM ${SRC}/module/media)

include_directories (${INC})

add_library(umodsys.libmedia STATIC
 ${S}/umodsys.lib.media.common.cpp
 ${S}/umodsys.lib.media.library.cpp
 
 ${SM}/stdio/umodsys.mod.media.stdio.cpp
 ${SM}/stdlib/umodsys.mod.media.stdlib.cpp
 ${SM}/zlib/umodsys.mod.media.zlib.cpp
 ${SM}/images_std/umodsys.mod.media.images_std.cpp
 ${SM}/serialize_json/umodsys.mod.media.serialize_json.cpp
)

