cmake_minimum_required(VERSION 3.14)
project(Lab05)

add_subdirectory(third-party/gtest)
add_executable(main main.cpp)
target_link_libraries(main gtest_main)
