cmake_minimum_required(VERSION 3.14)

project(mapcanvas LANGUAGES CXX)

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
# pkg_check_modules(CAIRO REQUIRED cairo)
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
  MapCanvas.h
  AffineTransformOperation.h
)

list(APPEND SOURCES 
  MapCanvas.cpp
  AffineTransformOperation.cpp
)

add_library(mapcanvas SHARED
  ${HEADERS}
  ${SOURCES}
)

message(STATUS geos ${GEOS_LIBRARIES})

target_link_libraries(mapcanvas 
  ${JSON_C_LIBRARIES}
  ${GEOS_LIBRARIES}
  ${UUID_LIBRARIES}
  ${OPENCV4_LIBRARIES}
  ${CAIRO_LIBRARIES}
  geos
  mapsymbol
)
target_compile_definitions(mapcanvas PRIVATE AS_LIBRARY)
target_include_directories(mapcanvas PUBLIC 
  .
  ../..
  ${JSON_C_INCLUDE_DIRS}
  ${GEOS_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${UUID_INCLUDE_DIRS}
  ${OPENCV4_INCLUDE_DIRS}
  ${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES}
  
)


target_link_directories(mapcanvas PUBLIC
  ${JSON_C_LIBRARY_DIRS}
  ${GEOS_LIBRARY_DIRS}
  ${CAIRO_LIBRARY_DIRS}
  ${UUID_LIBRARY_DIRS}
  ${OPENCV4_LIBRARY_DIRS}

)
target_compile_options(mapcanvas PUBLIC 
  ${JSON_C_CFLAGS_OTHER}
  ${GEOS_CFLAGS_OTHER}
  ${JSON_C_CFLAGS_OTHER}
  ${CAIRO_CFLAGS_OTHER}
  ${OPENCV4_LIBRARY_DIRS}
)

include(GNUInstallDirs)

install(TARGETS mapcanvas
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET mapcanvas POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mapcanvas> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:mapcanvas>
)

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mapcanvas
)
