#include "formatter.h"

std::string formatter(const std::string& message) {
    return "[" + message + "]";
}

void formatter(std::ostream& os, const std::string& message) {
    os << "[" << message << "]";
}
