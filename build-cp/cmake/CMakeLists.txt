
cmake_minimum_required (VERSION 2.8)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

project (UModSys)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

set(SRC ../../../src)
set(INC ../../../include)
set(PLAT $ENV{UMS_PLAT})
set(PID $ENV{UMS_PID})
set(PLATSRC platform/${PID})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../bin/${PLAT})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../bin/${PLAT})

#INCLUDE(FindPkgConfig)
#PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
#PKG_SEARCH_MODULE(SDL2TTF REQUIRED sdl2_ttf)
#FindSDL()
set(SDL2_BUILDING_LIBRARY true)
include(FindSDL2)
include(FindSDL2TTF)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -fPIC -fvisibility=hidden -Werror -D_FILE_OFFSET_BITS=64")
  message(STATUS "Forced PIC mode.")
endif()

message(STATUS "SDL2.LIB=${SDL2_LIBRARY} SDL2.INC=${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2TTF.LIB=${SDL2TTF_LIBRARY} SDL2TTF.INC=${SDL2TTF_INCLUDE_DIR}")

add_subdirectory(core)
add_subdirectory(base)

add_subdirectory(lib.2d)
add_subdirectory(lib.3d)
add_subdirectory(lib.media)
add_subdirectory(lib.ui)

add_subdirectory(plugin.tests)
add_subdirectory(basesys)

