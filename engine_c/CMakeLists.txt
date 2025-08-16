cmake_minimum_required(VERSION 3.14)

project(mapengine_c LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(UNIX)
    set(ENV{PKG_CONFIG_PATH} "/usr/local/pgsql/lib/pkgconfig:/usr/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
elseif(WIN32)
    set(ENV{PKG_CONFIG_PATH} "C:\\local\\lib\\pkgconfig;$ENV{PKG_CONFIG_PATH}")
endif()
# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)


find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON_C REQUIRED json-c)
pkg_check_modules(GEOS REQUIRED geos)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(UUID REQUIRED ossp-uuid)
pkg_check_modules(OPENCV4 REQUIRED opencv4)

# execute_process(COMMAND  pg_config --includedir
#     OUTPUT_VARIABLE PG_INC_DIR
# )
# execute_process(COMMAND  pg_config --libdir
#     OUTPUT_VARIABLE PG_LIB_DIR
# )
# execute_process(COMMAND  pg_config --bindir
#     OUTPUT_VARIABLE PG_BIN_DIR
# )
# string(REGEX REPLACE "\n$" "" PG_INC_DIR "${PG_INC_DIR}")
# string(REGEX REPLACE "\n$" "" PG_LIB_DIR "${PG_LIB_DIR}")
# string(REGEX REPLACE "\n$" "" PG_BIN_DIR "${PG_BIN_DIR}")

list(APPEND HEADERS
  mapengine_c.h
)
list(APPEND SOURCES
  mapengine_c.cpp
)


add_library(mapengine_c SHARED
  ${HEADERS}
  ${SOURCES}
)


target_link_libraries(mapengine_c 
  mapengine
)
target_compile_definitions(mapengine_c PRIVATE AS_LIBRARY)
target_include_directories(mapengine_c PUBLIC 
  .
  ../..
  ${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES}
)


target_link_directories(mapengine_c PUBLIC
  ../engine
)
target_compile_options(mapengine_c PUBLIC 
)

include(GNUInstallDirs)

install(TARGETS mapengine_c
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET mapengine_c POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mapengine_c> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:mapengine_c>
)

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mapengine_c
)



set(PROJECT_DESCRIPTION "MapEngine library provided by pcwang, C Wrapper.")
set(PROJECT_VERSION "1.0.0")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/mapengine_c.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/mapengine_c.pc"
    @ONLY
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/mapengine_c.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

