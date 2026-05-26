#include "solver.h"
#include <cmath>

solution solve_quadratic(double a, double b, double c) {
    double discriminant = b * b - 4 * a * c;
    std::complex<double> sqrt_disc = std::sqrt(std::complex<double>(discriminant, 0));
    return {
        (-b + sqrt_disc) / (2 * a),
        (-b - sqrt_disc) / (2 * a)
    };
}
