#!/bin/sh

echo "Tools to install skia library on ubuntu"

# 设置代理
export https_proxy=http://172.21.64.1:7897
export http_proxy=http://172.21.64.1:7897

echo "Clone skia repository from github"
SRC_DIR=~
cd $SRC_DIR
# git clone https://github.com/google/skia.git

cd skia
python3 tools/git-sync-deps

# bin/gn gen out/Release --args='
#     is_debug=false  
#     is_component_build=true 
#     skia_use_system_libjpeg_turbo=false 
#     skia_use_system_libpng=false 
#     skia_use_system_zlib=false 
#     skia_use_system_harfbuzz=false 
#     skia_use_system_expat=false 
#     skia_use_system_icu=false 
#     skia_use_system_libwebp=false 
#     skia_use_system_freetype2=false 
#     extra_cflags=["-fPIC"] 
# '
# ninja -C out/Release
