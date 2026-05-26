#include "formatter_ex.h"
#include "solver.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Solving: 1*x^2 - 3*x + 2 = 0" << std::endl;
    
    auto [root1, root2] = solve_quadratic(1, -3, 2);
    
    formatter_ex(std::cout, "Root 1: " + std::to_string(root1.real()));
    std::cout << std::endl;
    formatter_ex(std::cout, "Root 2: " + std::to_string(root2.real()));
    std::cout << std::endl;
    
    return 0;
}
