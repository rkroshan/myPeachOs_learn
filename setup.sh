#!/bin/sh
set -ev
CURRENT=$PWD
TOOLCHAIN_DIR=$CURRENT/toolchain
SRC=$TOOLCHAIN_DIR/src
BUILD=$TOOLCHAIN_DIR/build
# preparation exports
mkdir -p $SRC
mkdir -p $BUILD
export PREFIX=$BUILD
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
#install nasm and qemu for x86 bootloader development
sudo apt install -y nasm qemu-system-x86
sudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev aria2
#get bin-utils and gcc source for i686-elf tagret
cd $TOOLCHAIN_DIR
aria2c -x10 -c https://ftp.gnu.org/gnu/binutils/binutils-2.35.tar.xz
aria2c -x10 -c https://ftp.lip6.fr/pub/gcc/releases/gcc-10.2.0/gcc-10.2.0.tar.gz
tar -xf binutils-2.35.tar.xz
tar -xf gcc-10.2.0.tar.gz
#install bin utils
mkdir -p build-binutils
cd build-binutils
../binutils-2.35/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
#install gcc utils
cd $TOOLCHAIN_DIR
mkdir -p build-gcc
cd build-gcc
../gcc-10.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
cd $CURRENT
echo "ADD $PREFIX/bin to PATH in bashrc or make sure path is configured properly"
	