#!/bin/sh

echo "Tools to install skia library on ubuntu"

export https_proxy=http://172.21.64.1:7897
export http_proxy=http://172.21.64.1:7897

echo "Clone skia repository from github"
SRC_DIR=~
cd $SRC_DIR
# git clone https://github.com/google/skia.git

cd skia
# python3 tools/git-sync-deps

bin/gn gen out/Release --args='
    is_debug=false  
    is_component_build=true 
    skia_use_system_libjpeg_turbo=false 
    skia_use_system_libpng=false 
    skia_use_system_zlib=false 
    skia_use_system_expat=false 
    skia_use_system_icu=false 
    skia_use_system_libwebp=false 
    skia_use_system_freetype2=true
    skia_use_fontconfig=true
    skia_use_harfbuzz=true
    skia_use_system_harfbuzz=true
    skia_enable_svg=true
    skia_use_freetype=true
    skia_enable_tools=true
    extra_cflags=["-fPIC","-DSK_FONTMGR_FONTCONFIG_AVAILABLE"] 
'
ninja -C out/Release


# 打包库文件和头文件
rm -rf package
mkdir -p package/bin package/lib/pkgconfig package/include/skia/include 
cp -f out/Release/*.so package/lib
cp -rf include/* package/include/skia/include/

# 如果提供了安装路径则使用它，否则使用默认值
INSTALL_DIR="/usr/local"
INSTALL_DIR=${1:-$INSTALL_DIR}
# 创建 pkgconfig 文件
echo "prefix=${INSTALL_DIR}" > package/lib/pkgconfig/skia.pc
echo '
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: skia
Description: Skia Graphics Library
Version: 1.0.0
URL: https://skia.org/
Libs: -L${libdir} -lskia -lskparagraph -lskunicode_core -lskunicode_icu 
Libs.private: -lstdc++ -lpthread -ldl -lGL -lvulkan
Cflags: -I${includedir}/skia \
        -I${includedir}/skia/include \
        -I${includedir}/skia/include/core \
        -I${includedir}/skia/include/encode 
' >> package/lib/pkgconfig/skia.pc

# cat package/lib/pkgconfig/skia.pc

# 安装到系统目录
sudo mkdir -p $INSTALL_DIR
sudo cp -rf package/* $INSTALL_DIR
sudo chmod -R 755  $INSTALL_DIR