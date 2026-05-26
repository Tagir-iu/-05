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
