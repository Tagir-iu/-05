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
