cmake_minimum_required(VERSION 3.10)
project(tests)

set(CMAKE_CXX_STANDARD 11)

# Найти библиотеку Google Test
find_package(GTest REQUIRED)

# Добавить библиотеку из C-файлов
add_library(lw2_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lw2/src/point.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lw2/src/triangle.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lw2/src/thread_pool.c
)

# Добавить исполняемый файл для тестов
add_executable(tests lab2_test.cpp)

# Добавить директорию с заголовочными файлами
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw2/include)
target_include_directories(lw2_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw2/include)

# Слинковать с библиотекой Google Test и созданной библиотекой
target_link_libraries(tests GTest::GTest GTest::Main lw2_lib m)