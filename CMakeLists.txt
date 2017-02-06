project(KeyMaker)

cmake_minimum_required(VERSION 2.6)

set(KM_VERSION_MAJOR  0)
set(KM_VERSION_MINOR  1)
set(KM_VERSION_PATCH  0)

set(KM_VERSION  "${KM_VERSION_MAJOR}.${KM_VERSION_MINOR}.${KM_VERSION_PATCH}")

set(KM_DIR            ${PROJECT_SOURCE_DIR})
set(KM_SOURCE_DIR     ${KM_DIR}/src        )
set(KM_INCLUDE_DIR    ${KM_DIR}/include    )
set(KM_LIB_DIR        ${KM_DIR}/lib        )
set(KM_BUILD_DIR      ${KM_DIR}/bin        )
set(KM_TESTS_DIR      ${KM_DIR}/tests      )

set(LIBRARY_OUTPUT_PATH     ${KM_BUILD_DIR})
set(EXECUTABLE_OUTPUT_PATH  ${KM_BUILD_DIR})

add_definitions(
    -Wall
    -ansi
    -pedantic
    -pedantic-errors
    -Wno-deprecated-declarations
    -O2
    -std=c++17
)

set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_MULTITHREADED  ON )
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Threads)
find_package(PkgConfig)

pkg_check_modules(LIBSSH2 REQUIRED libssh2)
pkg_check_modules(LIBGIT  REQUIRED libgit2)

find_package(Boost 1.60 REQUIRED COMPONENTS system filesystem regex program_options)
find_package(OpenSSL    REQUIRED)
find_package(Lua 5.3    REQUIRED)
find_package(GTest      REQUIRED)

message(STATUS "Build Linenoise")

execute_process(
    COMMAND make -f ${KM_LIB_DIR}/linenoise.Makefile
    WORKING_DIRECTORY ${KM_LIB_DIR}/linenoise
    OUTPUT_QUIET
)

include_directories(
    ${KM_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
    ${LIBSSH2_INCLUDE_DIRS}
    ${LIBGIT_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIRS}
    ${KM_LIB_DIR}/termcolor/include
    ${KM_LIB_DIR}/linenoise
    ${KM_LIB_DIR}/sol2/single
    ${KM_LIB_DIR}/spdlog/include
)

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${OPENSSL_LIBRARY_DIRS}
    ${LIBSSH2_LIBRARY_DIRS}
    ${LIBGIT_LIBRARY_DIRS}
)

configure_file(
    ${KM_INCLUDE_DIR}/km/version.hpp.in
    ${KM_INCLUDE_DIR}/km/version.hpp
    IMMEDIATE @ONLY
)

add_library(keymaker STATIC
    ${KM_SOURCE_DIR}/Console.cpp
    ${KM_SOURCE_DIR}/Buffer.cpp
    ${KM_SOURCE_DIR}/RsaKey.cpp
    ${KM_SOURCE_DIR}/PrivateKey.cpp
    ${KM_SOURCE_DIR}/PublicKey.cpp
    ${KM_SOURCE_DIR}/AccessKey.cpp
    ${KM_SOURCE_DIR}/Encrypter.cpp
    ${KM_SOURCE_DIR}/TextNode.cpp
    ${KM_SOURCE_DIR}/PropertyNode.cpp
    ${KM_SOURCE_DIR}/KeyringNode.cpp
    ${KM_SOURCE_DIR}/EntryNode.cpp
    ${KM_SOURCE_DIR}/Repository.cpp
    ${KM_SOURCE_DIR}/Commit.cpp
    ${KM_SOURCE_DIR}/SshClient.cpp
    ${KM_SOURCE_DIR}/Core.cpp
    ${KM_SOURCE_DIR}/ScriptHandler.cpp
    ${KM_SOURCE_DIR}/Interpreter.cpp
    ${KM_SOURCE_DIR}/CommandLine.cpp
    ${KM_SOURCE_DIR}/ql/grammar.cpp
    ${KM_SOURCE_DIR}/lua/BufferWrapper.cpp
    ${KM_SOURCE_DIR}/lua/SshClientWrapper.cpp
    ${KM_SOURCE_DIR}/km.cpp
)

target_link_libraries(keymaker
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${LIBSSH2_LIBRARIES}
    ${LIBGIT_LIBRARIES}
    ${LUA_LIBRARIES}
    ${GTEST_LIBRARIES}
)

target_link_libraries(keymaker
    ${KM_LIB_DIR}/linenoise/linenoise.o
)

add_executable(km ${KM_SOURCE_DIR}/km.cpp)

target_link_libraries(km keymaker)

add_executable(km_tests
    ${KM_TESTS_DIR}/BufferTest.cpp
    ${KM_TESTS_DIR}/EncrypterTest.cpp
    ${KM_TESTS_DIR}/CoreTest.cpp
    ${KM_TESTS_DIR}/km_tests.cpp
)

target_link_libraries(km_tests
    keymaker
    ${GTEST_LIBRARIES}
)