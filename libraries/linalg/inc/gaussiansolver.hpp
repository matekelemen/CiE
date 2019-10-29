#ifndef LINALG_GAUSSIAN_SOLVER
#define LINALG_GAUSSIAN_SOLVER

#include <cmath>
#include <numeric>
#include <algorithm>

#include "matrix.hpp"
#include "linalgutilities.hpp"

namespace linalg {

// Solve linear system of equations
Vector solve( const Matrix& matrix,
              const Vector& vector );

}

#endif