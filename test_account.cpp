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
