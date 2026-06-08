cmake_minimum_required(VERSION 3.14)
project(Lab05)


enable_testing()

add_subdirectory(third-party/gtest)
add_subdirectory(formatter_lib)
add_subdirectory(solver_lib)


add_subdirectory(tests)
