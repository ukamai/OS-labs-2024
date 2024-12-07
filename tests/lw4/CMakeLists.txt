cmake_minimum_required(VERSION 3.10)
project(lab4_tests)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(lab4_test lab4_test.cpp)
target_link_libraries(lab4_test gtest gtest_main)

link_directories(${CMAKE_SOURCE_DIR}/lw4)

enable_testing()
add_test(NAME lab4_test COMMAND lab4_test)