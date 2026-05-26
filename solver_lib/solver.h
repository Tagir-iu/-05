#pragma once
#include <complex>
#include <utility>

using solution = std::pair<std::complex<double>, std::complex<double>>;
solution solve_quadratic(double a, double b, double c);
