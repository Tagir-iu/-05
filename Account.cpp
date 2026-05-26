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
