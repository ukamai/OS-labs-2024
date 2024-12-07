cmake_minimum_required(VERSION 3.10)
project(lw2)

set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(lw2 main.c src/point.c src/triangle.c src/thread_pool.c)

target_link_libraries(lw2 m)