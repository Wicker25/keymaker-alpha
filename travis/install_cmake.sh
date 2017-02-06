#!/usr/bin/env bash

CMAKE_VERSION="3.7.2"

set -e

sudo apt-get install build-essential
sudo apt-get remove cmake

cd tools

if [ ! -d "tools/cmake-${CMAKE_VERSION}" ]
then
  wget "https://cmake.org/files/v${CMAKE_VERSION%.*}/cmake-${CMAKE_VERSION}.tar.gz"
  tar xf "cmake-${CMAKE_VERSION}.tar.gz"
  rm "cmake-${CMAKE_VERSION}.tar.gz"

  cd "cmake-${CMAKE_VERSION}"
  ./configure --prefix=/usr
  make
  sudo make install
else
  cd "cmake-${CMAKE_VERSION}"
  sudo make install
fi

cd "${TRAVIS_BUILD_DIR}"