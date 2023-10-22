project(Google_tests)
cmake_minimum_required(VERSION 3.21)
add_subdirectory(lib/googletest)
#add_subdirectory(bot_tests)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run bot_tests.cpp)

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(Google_Tests_run deep_miner)

target_link_libraries(Google_Tests_run gtest gtest_main)