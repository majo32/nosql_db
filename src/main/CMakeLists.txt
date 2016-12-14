cmake_minimum_required (VERSION 2.8.4)

project (FRI_OS_NOSQLDB_Main)

add_subdirectory(../../src/main/client "client")
add_subdirectory(../../src/main/server "server")

add_subdirectory(../../lib/gtest-1.6.0 "build_gtest")


include_directories(../../lib/gtest-1.6.0/include)
include_directories(../../src/main/server/include)
include_directories(../../src/main/client/include)
include_directories(../../src/main/common/include)

