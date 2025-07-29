cmake_minimum_required(VERSION 3.14)

project(font_info LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)


find_package(PkgConfig REQUIRED)
pkg_check_modules(SKIA REQUIRED skia)

# pkg_check_modules(JSON_C REQUIRED json-c)
# pkg_check_modules(GEOS REQUIRED geos)
# find_package(Boost REQUIRED COMPONENTS json)

# pkg_check_modules(CAIRO REQUIRED cairo)

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
)

list(APPEND SOURCES 
  fonts_info.cpp
)

add_executable(fonts_info
  ${HEADERS}
  ${SOURCES}
)

target_link_libraries(fonts_info
  ${SKIA_LIBRARIES}
)


target_include_directories(fonts_info PUBLIC
  .
  ../..
  ${SKIA_INCLUDE_DIRS}
)
target_link_directories(fonts_info PUBLIC
  ${SKIA_LIBRARY_DIRS}
)
target_compile_options(fonts_info PUBLIC
  ${SKIA_CFLAGS_OTHER}
)

include(GNUInstallDirs)

install(TARGETS mapcanvas
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET fonts_info POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fonts_info> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:fonts_info>
)


