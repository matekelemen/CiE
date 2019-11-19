#ifndef LINALG_GAUSSIAN_SOLVER
#define LINALG_GAUSSIAN_SOLVER

#include <cmath>
#include <numeric>
#include <algorithm>

#include "../../types/inc/matrix.hpp"
#include "../../utilities/inc/linalgutilities.hpp"

namespace linalg {

// Solve linear system of equations
DoubleVector solve( const Matrix& matrix,
              const DoubleVector& vector );

}

#endif