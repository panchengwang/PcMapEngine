cmake_minimum_required(VERSION 3.14)
project(mapcanvas)
add_subdirectory(canvas)
add_subdirectory(symbol)
add_subdirectory(tools)

# mkdir build
# cd build
# cmake -DCMAKE_INSTALL_PREFIX=`pg_config --bindir`/.. -DCMAKE_BUILD_TYPE=Release ..
# cmake --build .
# cmake --build . --target install 
