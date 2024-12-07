cmake_minimum_required(VERSION 3.22)

project(mai-os)
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(CMAKE_CXX_STANDARD 17)

add_library(utils STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/src/utils.c
)

add_library(parent STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/src/parent.c
)

add_executable(lw1_test lab1_test.cpp)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/include)
target_include_directories(parent PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/include)
target_include_directories(lw1_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/include)

target_link_libraries(lw1_test PRIVATE utils parent GTest::GTest)

enable_testing()
add_test(NAME lw1_test COMMAND lw1_test)