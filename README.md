# myPeachOs_learn

install qemu and nasm compiler for bootloader
 - sudo apt install nasm qem-system-x86

install cross-compiler (steps verified on Ubuntu 18 and 20.04)

option 1: 
 - make setup and then add the toolchain/bin path to $PATH in bashrc

option 2:
 - https://wiki.osdev.org/GCC_Cross-Compiler
 - sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev
 - Download the source code 
 - (Binutils) wget https://ftp.gnu.org/gnu/binutils/binutils-2.35.tar.xz
 - (GCC) wget https://ftp.lip6.fr/pub/gcc/releases/gcc-10.2.0/gcc-10.2.0.tar.gz
 - Follow the build section on the GCC_cross-Compile page

Debugging points

gdb
 - sudo apt install gdb
 - make gdb
 - refer to top level make file for more information