cmake_minimum_required(VERSION 3.10)
project(lw4)

set(CMAKE_C_STANDARD 99)

# Указываем путь к заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR}/include)

# Создание библиотек
add_library(gcf_euclid SHARED src/gcf_euclid.c)
add_library(gcf_naive SHARED src/gcf_naive.c)
add_library(square_rectangle SHARED src/square_rectangle.c)
add_library(square_triangle SHARED src/square_triangle.c)

# Создание исполняемых файлов
add_executable(linking_main linking_main.c)
target_link_libraries(linking_main gcf_euclid square_rectangle)

add_executable(dynamic_main dynamic_main.c)
target_link_libraries(dynamic_main dl)