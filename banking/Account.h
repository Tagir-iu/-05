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
