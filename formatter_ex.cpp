#include "formatter_ex.h"
#include "formatter.h"

std::string formatter_ex(const std::string& message) {
    return formatter(message);
}

void formatter_ex(std::ostream& os, const std::string& message) {
    formatter(os, message);
}
