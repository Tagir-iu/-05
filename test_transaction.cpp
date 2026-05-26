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
