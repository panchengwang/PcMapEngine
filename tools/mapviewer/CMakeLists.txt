cmake_minimum_required(VERSION 3.14)

project(mapviewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)



find_package(PkgConfig REQUIRED)
pkg_check_modules(GEOS REQUIRED geos)


list(APPEND HEADERS 
)

list(APPEND SOURCES 
  mapviewer.cpp
)

add_executable(mapviewer
  ${HEADERS}
  ${SOURCES}
)

target_link_libraries(mapviewer
  ${GEOS_LIBRARIES}
  mapengine_c
)

target_include_directories(mapviewer PUBLIC
  .
  ../..
  ${GEOS_INCLUDE_DIRS}
)
target_link_directories(mapviewer PUBLIC
  ${GEOS_LIBRARY_DIRS}
)
target_compile_options(mapviewer PUBLIC
  ${GEOS_CFLAGS_OTHER}
)

include(GNUInstallDirs)

install(TARGETS mapviewer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET mapviewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mapviewer> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:mapviewer>
)


