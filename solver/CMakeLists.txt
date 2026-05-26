cmake_minimum_required(VERSION 3.10)
project(solver)

add_executable(solver main.cpp)
target_link_libraries(solver formatter_ex solver_lib)
install(TARGETS solver RUNTIME DESTINATION bin)
