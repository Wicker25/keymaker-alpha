#!/bin/sh

set -e
ROOT_PATH="${PWD}"

BOOST_PATH="${ROOT_PATH}/lib/boost"
OPENSSL_PATH="${ROOT_PATH}/lib/openssl"
LIBSSH2_PATH="${ROOT_PATH}/lib/libssh2"
LIBGIT2_PATH="${ROOT_PATH}/lib/libgit2"
LUA_PATH="${ROOT_PATH}/lib/lua"
GTEST_PATH="${ROOT_PATH}/lib/googletest"

export PKG_CONFIG_PATH="${OPENSSL_PATH}/install/lib/pkgconfig"
export PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:${LIBSSH2_PATH}/install/lib/pkgconfig:${LIBSSH2_PATH}/install/lib64/pkgconfig"
export PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:${LIBGIT2_PATH}/install/lib/pkgconfig"

export LUA_DIR="${LUA_PATH}/install"

cmake \
  -DBOOST_ROOT="${BOOST_PATH}/install" \
  -DOPENSSL_ROOT_DIR="${OPENSSL_PATH}/install" \
  -DGTEST_ROOT="${GTEST_PATH}/install" \
  .

make