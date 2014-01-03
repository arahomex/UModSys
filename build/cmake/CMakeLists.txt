
cmake_minimum_required (VERSION 2.8)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

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
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../bin/${PLAT})

add_subdirectory(core)
add_subdirectory(base)

#2add_subdirectory(lib.2d)
#add_subdirectory(lib.3d)
add_subdirectory(lib.media)
#add_subdirectory(lib.ui)

#add_subdirectory(plugin.tests)
add_subdirectory(basesys)

