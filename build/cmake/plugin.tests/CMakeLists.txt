
set(SM ${SRC}/plugin/test)

include_directories (${INC})


add_library(umodsys.plugin.test1 SHARED
  ${SM}/plugin_test1.cpp
)
target_link_libraries(umodsys.plugin.test1
  umodsys.libmedia
  umodsys.lib2d
  umodsys.lib3d
  umodsys.libui
  umodsys.base
  umodsys.core
  ${SDL2_LIBRARY}
  ${SDL2TTF_LIBRARY}
  png
  jpeg
  z
)

add_library(umodsys.plugin.test2 SHARED
  ${SM}/plugin_test2.cpp
)
target_link_libraries(umodsys.plugin.test2
  umodsys.libmedia
  umodsys.lib2d
  umodsys.lib3d
  umodsys.libui
  umodsys.base
  umodsys.core
  ${SDL2_LIBRARY}
)


