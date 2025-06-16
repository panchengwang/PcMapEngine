#!/bin/sh

echo "Tools to uninstall skia library on ubuntu"
# 如果提供了安装路径则使用它，否则使用默认值
INSTALL_DIR="/usr/local"
INSTALL_DIR=${1:-$INSTALL_DIR}


# 删除skia文件

sudo rm -rf $INSTALL_DIR/include/skia
sudo rm -rf $INSTALL_DIR/lib/libskia.so $INSTALL_DIR/lib/libsk*.so 
sudo rm -rf $INSTALL_DIR/lib/pkgconfig/skia.pc