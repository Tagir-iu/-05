cmake_minimum_required(VERSION 3.10)
project(hello_world)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello_world main.cpp)

# Пути к заголовкам
target_include_directories(hello_world PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../formatter_lib
)

# Линкуем ОБЕ библиотеки (formatter И formatter_ex)
target_link_libraries(hello_world PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/libformatter_ex.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../formatter_lib/build/libformatter.a
)
