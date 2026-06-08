cmake_minimum_required(VERSION 3.14)
project(lab05)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

# Подключаем основную библиотеку
add_subdirectory(banking)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(third-party/gtest)

    # Создаем исполняемый файл тестов
    add_executable(check tests/test_account.cpp tests/test_transaction.cpp)

    # Привязываем библиотеки в правильном для Clang порядке
    target_link_libraries(check gtest_main banking)
    target_include_directories(check PRIVATE banking)

    # Включаем покрытие БЕЗОПАСНО — только для тестов и самой библиотеки banking
    target_compile_options(check PRIVATE --coverage)
    target_link_options(check PRIVATE --coverage)
    
    target_compile_options(banking PRIVATE --coverage)
    target_link_options(banking PRIVATE --coverage)

    add_test(NAME check COMMAND check)
endif()
