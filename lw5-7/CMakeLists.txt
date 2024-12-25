cmake_minimum_required(VERSION 3.22)

project(mai-os)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Подключение Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

# Добавление библиотек
add_library(lab5_7_tools src/tools.cpp)
add_library(lab5_7_worker src/worker.cpp)
add_library(lab5_7_controller src/controller.cpp)

# Указание путей к заголовочным файлам
target_include_directories(lab5_7_tools PUBLIC include)
target_include_directories(lab5_7_worker PUBLIC include)
target_include_directories(lab5_7_controller PUBLIC include)

# Линковка библиотек
target_link_libraries(lab5_7_tools PRIVATE /usr/lib/x86_64-linux-gnu/libzmq.so)
target_link_libraries(lab5_7_worker PRIVATE /usr/lib/x86_64-linux-gnu/libzmq.so)
target_link_libraries(lab5_7_controller PRIVATE /usr/lib/x86_64-linux-gnu/libzmq.so lab5_7_tools lab5_7_worker)

# Добавление исполняемого файла
add_executable(lab5_7 main.cpp)
target_include_directories(lab5_7 PUBLIC include)
target_link_libraries(lab5_7 PRIVATE lab5_7_controller /usr/lib/x86_64-linux-gnu/libzmq.so)

# Добавление тестового файла
add_executable(test_node_creation test_node_creation.cpp)
target_include_directories(test_node_creation PUBLIC include)
target_link_libraries(test_node_creation PRIVATE lab5_7_tools lab5_7_worker lab5_7_controller /usr/lib/x86_64-linux-gnu/libzmq.so GTest::gtest GTest::gtest_main)

# Включение тестирования
enable_testing()
add_test(NAME test_node_creation COMMAND test_node_creation)