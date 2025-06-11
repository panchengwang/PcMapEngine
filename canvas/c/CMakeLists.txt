cmake_minimum_required(VERSION 3.14)

project(mapcanvas_c LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)


find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON_C REQUIRED json-c)
pkg_check_modules(CAIRO REQUIRED cairo)


list(APPEND HEADERS 
    canvas_c.h
)

list(APPEND SOURCES 
	canvas_c.c
)

add_library(mapcanvas_c SHARED
  ${HEADERS}
  ${SOURCES}
)

target_link_directories(mapcanvas_c PRIVATE
    ../cpp
)

target_link_libraries(mapcanvas_c 
	mapcanvas
)
target_compile_definitions(mapcanvas_c PRIVATE AS_LIBRARY)
target_include_directories(mapcanvas_c PUBLIC 
	../cpp
)



include(GNUInstallDirs)

install(TARGETS mapcanvas_c
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET mapcanvas_c POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mapcanvas_c> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:mapcanvas_c>
)

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mapcanvas_c
)
