#!/bin/sh

ROOT_PATH="${PWD}"

BOOST_VERSION="1.63.0"
LUA_VERSION="5.3.4"

set -e

# Retrieve some dependencies by using git
git submodule update --init --recursive

# Build Boost
if [ ! -d "lib/boost" ]
then
  cd lib

  wget \
    -O "boost_${BOOST_VERSION//./_}.tar.bz2" \
    "https://sourceforge.net/projects/boost/files/boost/${BOOST_VERSION}/boost_${BOOST_VERSION//./_}.tar.bz2"

  tar -xjf "boost_${BOOST_VERSION//./_}.tar.bz2"
  rm "boost_${BOOST_VERSION//./_}.tar.bz2"
  mv "boost_${BOOST_VERSION//./_}" boost

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
  cd lib

  wget \
    -O "lua-${LUA_VERSION}.tar.gz" \
    "http://www.lua.org/ftp/lua-${LUA_VERSION}.tar.gz"

  tar -xzf "lua-${LUA_VERSION}.tar.gz"
  rm "lua-${LUA_VERSION}.tar.gz"
  mv "lua-${LUA_VERSION}" lua

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