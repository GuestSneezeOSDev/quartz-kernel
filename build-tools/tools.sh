#!/bin/bash

set -e

sudo apt-get update
sudo apt-get install -y build-essential gcc-multilib g++-multilib nasm grub2-bin curl make

mkdir -p /tmp/src
cd /tmp/src
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.34.tar.gz
tar xf binutils-2.34.tar.gz
cd binutils-2.34
mkdir build
cd build
../configure --target=x86_64-elf --prefix=/opt/cross --with-sysroot --disable-nls --disable-werror
make
make install

cd /tmp/src
curl -O http://ftp.gnu.org/gnu/gcc/gcc-9.3.0/gcc-9.3.0.tar.gz
tar xf gcc-9.3.0.tar.gz
cd gcc-9.3.0
mkdir build
cd build
../configure --target=x86_64-elf --prefix=/opt/cross --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
