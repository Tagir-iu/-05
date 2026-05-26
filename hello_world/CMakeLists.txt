cmake_minimum_required(VERSION 3.10)
project(hello_world)

add_executable(hello_world main.cpp)
target_link_libraries(hello_world formatter_ex)
install(TARGETS hello_world RUNTIME DESTINATION bin)
