
set(S ${SRC}/lib/ui)
set(SM ${SRC}/module/ui)

include_directories(${INC} ${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})

add_library(umodsys.libui STATIC
 ${S}/umodsys.lib.ui.common.cpp
 
 ${SM}/frames/umodsys.mod.ui.frames.cpp
 ${SM}/SDL_core/umodsys.mod.ui.SDL_core.cpp
# ${SM}/SDL_GL/umodsys.mod.ui.SDL_GL.cpp
)

