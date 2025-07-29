cmake_minimum_required(VERSION 3.14)

project(mapsymbol LANGUAGES CXX)

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
pkg_check_modules(CAIRO REQUIRED cairo)
# pkg_check_modules(GEOS REQUIRED geos)
# pkg_check_modules(SKIA REQUIRED skia)
# pkg_check_modules(UUID REQUIRED ossp-uuid)
pkg_check_modules(OPENCV4 REQUIRED opencv4)
# find_package(Boost REQUIRED COMPONENTS json)

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
  JsonUtils.h
  SymColor.h
  SymStroke.h
  SymFill.h
  SymSolidFill.h
  SymPoint.h  
  SymShape.h
  SymSystemLine.h
  SymArc.h
  SymChord.h
  SymPie.h
  SymCircle.h
  SymEllipse.h
  SymRectangle.h
  SymLineString.h
  SymPolygon.h
  SymRegularPolygon.h
  SymStar.h
  SymText.h 
  SymCanvas.h
  SymMath.h
  MapSymbol.h

)

list(APPEND SOURCES 
  SymColor.cpp
  SymStroke.cpp
  SymFill.cpp
  SymSolidFill.cpp
  SymPoint.cpp
  SymShape.cpp
  SymSystemLine.cpp
  SymArc.cpp
  SymChord.cpp
  SymPie.cpp
  SymCircle.cpp
  SymEllipse.cpp
  SymRectangle.cpp
  SymLineString.cpp
  SymPolygon.cpp 
  SymRegularPolygon.cpp
  SymStar.cpp
  SymText.cpp
  SymCanvas.cpp
  MapSymbol.cpp
)

add_library(mapsymbol SHARED
  ${HEADERS}
  ${SOURCES}
)

# message(STATUS geos ${GEOS_LIBRARIES})

target_link_libraries(mapsymbol 
  ${JSON_C_LIBRARIES}
  ${CAIRO_LIBRARIES}
  ${OPENCV4_LIBRARIES}
)
target_compile_definitions(mapsymbol PRIVATE AS_LIBRARY)
target_include_directories(mapsymbol PUBLIC 
  .
  ../..
  ${JSON_C_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  # ${UUID_INCLUDE_DIRS}
  ${OPENCV4_INCLUDE_DIRS}
  ${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES}
)


target_link_directories(mapsymbol PUBLIC
  ${JSON_C_LIBRARY_DIRS}
  ${CAIRO_LIBRARY_DIRS}
  ${OPENCV4_LIBRARY_DIRS}
)
target_compile_options(mapsymbol PUBLIC 
  ${JSON_C_CFLAGS_OTHER}
  ${CAIRO_CFLAGS_OTHER}
  ${OPENCV4_CFLAGS_OTHER}
)

include(GNUInstallDirs)

install(TARGETS mapsymbol
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET mapsymbol POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mapsymbol> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:mapsymbol>

)

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mapsymbol
)
