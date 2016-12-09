cmake_minimum_required (VERSION 2.8.4)

project (FRI_OS_NOSQLDB_UnitTest)

add_subdirectory(../../src/main/client "build_client")
add_subdirectory(../../src/main/server "build_server")

add_subdirectory(../../lib/gtest-1.6.0 "build_gtest")


include_directories(../../lib/gtest-1.6.0/include)
include_directories(../../src/main/server/include)
include_directories(../../src/main/client/include)
include_directories(../../src/main/common/include)

enable_testing()


file(GLOB_RECURSE TEST_SRC_FILES *.cpp)

add_executable(FRI_OS_NOSQLDB_UnitTest ${TEST_SRC_FILES})

target_link_libraries(FRI_OS_NOSQLDB_UnitTest gtest gtest_main)
target_link_libraries(FRI_OS_NOSQLDB_UnitTest FRI_OS_NOSQLDB_Server_lib_bundle)
#target_link_libraries(FRI_OS_NOSQLDB_UnitTest FRI_OS_NOSQLDB_Client)
