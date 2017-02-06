#!/bin/sh

set -e
ROOT_PATH="${PWD}"

# Retrieve some dependencies by using git
git submodule update --init --recursive

# Build Boost
if [ ! -d "lib/boost" ]
then
  cd lib/
  wget -O boost_1_63_0.tar.bz2 https://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.bz2
  tar -xjf boost_1_63_0.tar.bz2
  rm boost_1_63_0.tar.bz2
  mv boost_1_63_0 boost
  cd boost
  mkdir install/
  ./bootstrap.sh --prefix="${PWD}/install"
  ./b2 install cxxflags="-I/usr/include/python2.7/"
  cd "${ROOT_PATH}"
fi

# Build OpenSSL
if [ ! -f "lib/openssl/Makefile" ]
then
  cd lib/openssl
  mkdir install/
  ./config --prefix="${PWD}/install"
  make
  make install
  cd "${ROOT_PATH}"
fi

# Build libssh2
if [ ! -f "lib/libssh2/Makefile" ]
then
  cd lib/libssh2
  mkdir install/
  cmake -DCMAKE_INSTALL_PREFIX="${PWD}/install" -DOPENSSL_ROOT_DIR="${ROOT_PATH}/lib/openssl/install" .
  make
  make install
  cd "${ROOT_PATH}"
fi

# Build libgit2
if [ ! -f "lib/libgit2/Makefile" ]
then
  cd lib/libgit2
  mkdir install/
  cmake -DCMAKE_INSTALL_PREFIX="${PWD}/install" .
  make
  make install
  cd "${ROOT_PATH}"
fi

# Build Lua
if [ ! -d "lib/lua" ]
then
  cd lib/
  wget -O lua-5.3.4.tar.gz http://www.lua.org/ftp/lua-5.3.4.tar.gz
  tar -xzf lua-5.3.4.tar.gz
  rm lua-5.3.4.tar.gz
  mv lua-5.3.4 lua
  cd lua
  make linux
  make local
  cd "${ROOT_PATH}"
fi

# Build Google Test
if [ ! -f "lib/googletest/Makefile" ]
then
  cd lib/googletest
  cmake -DCMAKE_INSTALL_PREFIX="${PWD}/install" .
  make
  make install
  cd "${ROOT_PATH}"
fi