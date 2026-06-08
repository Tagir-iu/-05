# Отчет по лабораторной работе №5
ИНТЕГРАЦИЯ ci/cd: [![Coverage Status](https://coveralls.io/repos/github/Tagir-iu/-05/badge.svg?branch=main)](https://coveralls.io/github/Tagir-iu/-05?branch=main)

**Студент:** Tagir-iu
**Тема:** Модульное тестирование с GTest и Coveralls.io


## 1. Цель работы

Научиться использовать фреймворк Google Test для модульного тестирования C++ проектов,
настроить непрерывную интеграцию (CI) через GitHub Actions и сервис Coveralls.io
для отслеживания покрытия кода.


## 2. Выполнение работы

### 2.1. Добавление Google Test

В проект был добавлен Google Test как git submodule:

mkdir -p third-party
git submodule add https://github.com/google/googletest third-party/gtest
cd third-party/gtest && git checkout release-1.8.1 && cd ../..

### 2.2. Создание библиотеки banking

Структура библиотеки:
```
banking/
├── Account.h         # класс банковского счета
├── Account.cpp       # реализация методов счета
├── Transaction.h     # класс транзакции
├── Transaction.cpp   # реализация транзакции
└── CMakeLists.txt    # сборка библиотеки
```
#### Account.h
```
#pragma once
#include <string>

class Account {
private:
    std::string id;
    double balance;
public:
    Account(const std::string& id, double initialBalance = 0.0);
    void deposit(double amount);
    bool withdraw(double amount);
    double getBalance() const;
    std::string getId() const;
};
```
#### Account.cpp
```
#include "Account.h"
#include <stdexcept>

Account::Account(const std::string& id, double initialBalance)
    : id(id), balance(initialBalance) {}

void Account::deposit(double amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Deposit amount must be positive");
    }
    balance += amount;
}

bool Account::withdraw(double amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Withdraw amount must be positive");
    }
    if (amount > balance) {
        return false;
    }
    balance -= amount;
    return true;
}

double Account::getBalance() const {
    return balance;
}

std::string Account::getId() const {
    return id;
}
```
#### Transaction.h
```
#pragma once
#include "Account.h"
#include <string>

class Transaction {
private:
    std::string fromId;
    std::string toId;
    double amount;
    bool completed;
public:
    Transaction(const std::string& from, const std::string& to, double amount);
    bool execute(Account& from, Account& to);
    bool isCompleted() const;
    double getAmount() const;
};
```
#### Transaction.cpp
```
#include "Transaction.h"
#include <stdexcept>

Transaction::Transaction(const std::string& from, const std::string& to, double amount)
    : fromId(from), toId(to), amount(amount), completed(false) {
    if (amount <= 0) {
        throw std::invalid_argument("Transaction amount must be positive");
    }
}

bool Transaction::execute(Account& from, Account& to) {
    if (completed) {
        return false;
    }
    
    if (from.getId() != fromId || to.getId() != toId) {
        return false;
    }
    
    if (from.withdraw(amount)) {
        to.deposit(amount);
        completed = true;
        return true;
    }
    return false;
}

bool Transaction::isCompleted() const {
    return completed;
}

double Transaction::getAmount() const {
    return amount;
}
```
#### banking/CMakeLists.txt
```
add_library(banking STATIC Account.cpp Transaction.cpp)
target_include_directories(banking PUBLIC .)
install(TARGETS banking ARCHIVE DESTINATION lib)
install(FILES Account.h Transaction.h DESTINATION include)
```
### 2.3. Создание модульных тестов

Тесты размещены в директории tests/:
```
tests/
├── test_account.cpp      # тесты для класса Account
└── test_transaction.cpp  # тесты для класса
```
Transaction

#### test_account.cpp
```
#include <gtest/gtest.h>
#include "Account.h"

TEST(AccountTest, ConstructorInitializesBalance) {
    Account acc("123", 100.0);
    EXPECT_EQ(acc.getBalance(), 100.0);
    EXPECT_EQ(acc.getId(), "123");
}

TEST(AccountTest, DepositIncreasesBalance) {
    Account acc("123", 100.0);
    acc.deposit(50.0);
    EXPECT_EQ(acc.getBalance(), 150.0);
}

TEST(AccountTest, DepositNegativeThrows) {
    Account acc("123", 100.0);
    EXPECT_THROW(acc.deposit(-10.0), std::invalid_argument);
}

TEST(AccountTest, WithdrawDecreasesBalance) {
    Account acc("123", 100.0);
    bool success = acc.withdraw(30.0);
    EXPECT_TRUE(success);
    EXPECT_EQ(acc.getBalance(), 70.0);
}

TEST(AccountTest, WithdrawMoreThanBalanceFails) {
    Account acc("123", 100.0);
    bool success = acc.withdraw(150.0);
    EXPECT_FALSE(success);
    EXPECT_EQ(acc.getBalance(), 100.0);
}

TEST(AccountTest, WithdrawNegativeThrows) {
    Account acc("123", 100.0);
    EXPECT_THROW(acc.withdraw(-10.0), std::invalid_argument);
}
```
#### test_transaction.cpp
```
#include <gtest/gtest.h>
#include "Account.h"
#include "Transaction.h"

TEST(TransactionTest, ExecuteTransfersMoney) {
    Account from("A", 100.0);
    Account to("B", 0.0);
    Transaction tx("A", "B", 50.0);
    
    bool success = tx.execute(from, to);
    
    EXPECT_TRUE(success);
    EXPECT_TRUE(tx.isCompleted());
    EXPECT_EQ(from.getBalance(), 50.0);
    EXPECT_EQ(to.getBalance(), 50.0);
}

TEST(TransactionTest, ExecuteFailsIfInsufficientFunds) {
    Account from("A", 30.0);
    Account to("B", 0.0);
    Transaction tx("A", "B", 50.0);
    
    bool success = tx.execute(from, to);
    
    EXPECT_FALSE(success);
    EXPECT_FALSE(tx.isCompleted());
    EXPECT_EQ(from.getBalance(), 30.0);
    EXPECT_EQ(to.getBalance(), 0.0);
}

TEST(TransactionTest, ExecuteFailsIfAlreadyCompleted) {
    Account from("A", 100.0);
    Account to("B", 0.0);
    Transaction tx("A", "B", 50.0);
    
    tx.execute(from, to);
    bool second = tx.execute(from, to);
    
    EXPECT_FALSE(second);
}

TEST(TransactionTest, ExecuteFailsWithWrongAccounts) {
    Account from1("A", 100.0);
    Account to1("B", 0.0);
    Account from2("C", 100.0);
    Account to2("D", 0.0);
    Transaction tx("A", "B", 50.0);
    
    bool success = tx.execute(from2, to2);
    
    EXPECT_FALSE(success);
}
```
### 2.4. Настройка корневого CMakeLists.txt
```
cmake_minimum_required(VERSION 3.14)
project(lab05)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(banking)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(third-party/gtest)

    add_executable(check tests/test_account.cpp tests/test_transaction.cpp)

    target_link_libraries(check gtest_main banking)
    target_include_directories(check PRIVATE banking)

    target_compile_options(check PRIVATE --coverage)
    target_link_options(check PRIVATE --coverage)
    
    target_compile_options(banking PRIVATE --coverage)
    target_link_options(banking PRIVATE --coverage)

    add_test(NAME check COMMAND check)
endif()
```
### 2.5. Настройка GitHub Actions

Файл .github/workflows/linux.yml:

name: Linux CI (gcc & clang)
```
name: Linux CI (gcc & clang)

on:
  push:
    branches: [ main, master ]
  pull_request:
    branches: [ main, master ]

jobs:
  build:
    runs-on: ubuntu-22.04

    strategy:
      matrix:
        compiler: [gcc, clang]

    env:
      CC: ${{ matrix.compiler }}
      CXX: ${{ matrix.compiler == 'gcc' && 'g++' || 'clang++' }}

    steps:
      - uses: actions/checkout@v4

      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y cmake build-essential lcov llvm

      - name: Configure with tests
        run: cmake -H. -B_build -DBUILD_TESTS=ON

      - name: Build
        run: cmake --build _build

      - name: Run tests
        run: ctest --test-dir _build --output-on-failure

      - name: Generate coverage report
        run: |
          if [ "${{ matrix.compiler }}" = "clang" ]; then
            echo '#!/bin/sh' > llvm-gcov.sh
            echo 'exec llvm-cov gcov "$@"' >> llvm-gcov.sh
            chmod +x llvm-gcov.sh
            GCOV_ARG="--gcov-tool $(pwd)/llvm-gcov.sh"
          else
            GCOV_ARG=""
          fi

          lcov --directory _build --capture --output-file coverage.info $GCOV_ARG
          lcov --remove coverage.info '/usr/*' '*/third-party/*' '*/tests/*' --output-file coverage.info
          lcov --list coverage.info

      - name: Upload to Coveralls
        uses: coverallsapp/github-action@v2
        with:
          github-token: ${{ secrets.GITHUB_TOKEN }}
          file: coverage.info
          flag-name: ${{ matrix.compiler }}
          parallel: true

  finish:
    needs: build
    runs-on: ubuntu-latest
    steps:
      - name: Close Coveralls parallel build
        uses: coverallsapp/github-action@v2
        with:
          github-token: ${{ secrets.GITHUB_TOKEN }}
          parallel-finished: true
```
### 2.6. Настройка Coveralls.io

1. Зарегистрировался на https://coveralls.io через GitHub
2. Добавил репозиторий qNetayS/lab05
3. Включил автоматическую отправку покрытия



## 3. Результаты

### 3.1. Результаты тестирования

Все тесты успешно пройдены:
```
[==========] Running 10 tests from 2 test suites.
[----------] 6 tests from AccountTest
[ RUN      ] AccountTest.ConstructorInitializesBalance
[       OK ] AccountTest.ConstructorInitializesBalance (0 ms)
[ RUN      ] AccountTest.DepositIncreasesBalance
[       OK ] AccountTest.DepositIncreasesBalance (0 ms)
[ RUN      ] AccountTest.DepositNegativeThrows
[       OK ] AccountTest.DepositNegativeThrows (0 ms)
[ RUN      ] AccountTest.WithdrawDecreasesBalance
[       OK ] AccountTest.WithdrawDecreasesBalance (0 ms)
[ RUN      ] AccountTest.WithdrawMoreThanBalanceFails
[       OK ] AccountTest.WithdrawMoreThanBalanceFails (0 ms)
[ RUN      ] AccountTest.WithdrawNegativeThrows
[       OK ] AccountTest.WithdrawNegativeThrows (0 ms)
[----------] 6 tests from AccountTest (0 ms total)

[----------] 4 tests from TransactionTest
[ RUN      ] TransactionTest.ExecuteTransfersMoney
[       OK ] TransactionTest.ExecuteTransfersMoney (0 ms)
[ RUN      ] TransactionTest.ExecuteFailsIfInsufficientFunds
[       OK ] TransactionTest.ExecuteFailsIfInsufficientFunds (0 ms)
[ RUN      ] TransactionTest.ExecuteFailsIfAlreadyCompleted
[       OK ] TransactionTest.ExecuteFailsIfAlreadyCompleted (0 ms)
[ RUN      ] TransactionTest.ExecuteFailsWithWrongAccounts
[       OK ] TransactionTest.ExecuteFailsWithWrongAccounts (0 ms)
[----------] 4 tests from TransactionTest (0 ms total)

[==========] 10 tests from 2 test suites ran. (0 ms total)
[  PASSED  ] 10 tests.
```
### 3.2. Покрытие кода

Покрытие кода составляет 92%
