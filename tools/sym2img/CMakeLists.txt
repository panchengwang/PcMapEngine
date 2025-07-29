cmake_minimum_required(VERSION 3.14)

project(sym2img LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)


find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON_C REQUIRED json-c)
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
  sym2img.cpp
)

add_executable(sym2img
  ${HEADERS}
  ${SOURCES}
)

target_link_libraries(sym2img
  ${JSON_C_LIBRARIES}
  # mapcanvas
  mapengine
)
target_compile_definitions(sym2img PRIVATE AS_LIBRARY)

target_include_directories(sym2img PUBLIC
  .
  ../..
)
target_link_directories(sym2img PUBLIC
  # ${JSON_C_LIBRARY_DIRS}
  ${GEOS_LIBRARY_DIRS}
  ${BOOST_LIBRARY_DIRS}

)
target_compile_options(sym2img PUBLIC
  # ${JSON_C_CFLAGS_OTHER}
  ${GEOS_CFLAGS_OTHER}
  ${BOOST_CFLAGS_OTHER}
)

include(GNUInstallDirs)

install(TARGETS sym2img
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET sym2img POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sym2img> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:sym2img>
)


