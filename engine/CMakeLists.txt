cmake_minimum_required(VERSION 3.14)

project(mapengine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(UNIX)
    set(ENV{PKG_CONFIG_PATH} "/usr/local/pgsql/lib/pkgconfig:/usr/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
elseif(WIN32)
    set(ENV{PKG_CONFIG_PATH} "C:\\local\\lib\\pkgconfig;$ENV{PKG_CONFIG_PATH}")
endif()
# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)

find_package( Eigen3 REQUIRED )
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

list(APPEND COMMON_HEADERS
  JsonUtils.h
  SymMath.h
  Serialize.h
)
list(APPEND SYMBOL_HEADERS
  symbol/SymColor.h
  symbol/SymStroke.h
  symbol/SymFill.h
  symbol/SymSolidFill.h
  symbol/SymPoint.h  
  symbol/SymShape.h
  symbol/SymSystemLine.h
  symbol/SymSystemFill.h
  symbol/SymArc.h
  symbol/SymChord.h
  symbol/SymPie.h
  symbol/SymCircle.h
  symbol/SymEllipse.h
  symbol/SymRectangle.h
  symbol/SymLineString.h
  symbol/SymPolygon.h
  symbol/SymRegularPolygon.h
  symbol/SymStar.h
  symbol/SymText.h 
  symbol/SymCanvas.h
  symbol/MapSymbol.h
)

list(APPEND SYMBOL_SOURCES
  symbol/SymColor.cpp
  symbol/SymStroke.cpp
  symbol/SymFill.cpp
  symbol/SymSolidFill.cpp
  symbol/SymPoint.cpp
  symbol/SymShape.cpp
  symbol/SymSystemLine.cpp
  symbol/SymSystemFill.cpp
  symbol/SymArc.cpp
  symbol/SymChord.cpp
  symbol/SymPie.cpp
  symbol/SymCircle.cpp
  symbol/SymEllipse.cpp
  symbol/SymRectangle.cpp
  symbol/SymLineString.cpp
  symbol/SymPolygon.cpp 
  symbol/SymRegularPolygon.cpp
  symbol/SymStar.cpp
  symbol/SymText.cpp
  symbol/SymCanvas.cpp
  symbol/MapSymbol.cpp
)

list(APPEND CANVAS_HEADERS    
  canvas/MapCanvas.h
  canvas/AffineTransformer.h
  canvas/AffineTransformOperation.h
)

list(APPEND CANVAS_SOURCES 
  canvas/MapCanvas.cpp
  canvas/AffineTransformer.cpp
  canvas/AffineTransformOperation.cpp
)

set(HEADERS ${SYMBOL_HEADERS} ${CANVAS_HEADERS})
set(SOURCES ${SYMBOL_SOURCES} ${CANVAS_SOURCES})
add_library(mapengine SHARED
  ${HEADERS}
  ${SOURCES}
)


target_link_libraries(mapengine 
  ${JSON_C_LIBRARIES}
  ${GEOS_LIBRARIES}
  ${UUID_LIBRARIES}
  ${OPENCV4_LIBRARIES}
  ${CAIRO_LIBRARIES}
  ${EIGEN3_LIBRARIES}
  geos
)
target_compile_definitions(mapengine PRIVATE AS_LIBRARY)
target_include_directories(mapengine PUBLIC 
  .
  ../..
  canvas 
  symbol
  ${JSON_C_INCLUDE_DIRS}
  ${GEOS_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${UUID_INCLUDE_DIRS}
  ${OPENCV4_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
  ${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES}
  
)


target_link_directories(mapengine PUBLIC
  ${JSON_C_LIBRARY_DIRS}
  ${GEOS_LIBRARY_DIRS}
  ${CAIRO_LIBRARY_DIRS}
  ${UUID_LIBRARY_DIRS}
  ${OPENCV4_LIBRARY_DIRS}
  ${EIGEN3_LIBRARY_DIRS}
)

target_compile_options(mapengine PUBLIC 
  ${JSON_C_CFLAGS_OTHER}
  ${GEOS_CFLAGS_OTHER}
  ${JSON_C_CFLAGS_OTHER}
  ${CAIRO_CFLAGS_OTHER}
  ${OPENCV4_LIBRARY_OTHER}
  ${EIGEN3_LIBRARY_OTHER}
)

include(GNUInstallDirs)

install(TARGETS mapengine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET mapengine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mapengine> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:mapengine>
)

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mapengine
)

set(PROJECT_DESCRIPTION "MapEngine library provided by pcwang.")
set(PROJECT_VERSION "1.0.0")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/mapengine.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/mapengine.pc"
    @ONLY
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/mapengine.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
