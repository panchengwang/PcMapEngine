cmake_minimum_required(VERSION 3.14)

project(sym2img_c LANGUAGES C)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)


find_package(PkgConfig REQUIRED)


list(APPEND HEADERS 
)

list(APPEND SOURCES 
  sym2img_c.c
)

add_executable(sym2img_c
  ${HEADERS}
  ${SOURCES}
)

target_link_libraries(sym2img_c
  # ${JSON_C_LIBRARIES}
  # mapcanvas
  mapengine_c
)

target_include_directories(sym2img_c PUBLIC
  .
  ../..
)
target_link_directories(sym2img_c PUBLIC
)
target_compile_options(sym2img_c PUBLIC
)

include(GNUInstallDirs)

install(TARGETS sym2img_c
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET sym2img_c POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sym2img_c> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:sym2img_c>
)


