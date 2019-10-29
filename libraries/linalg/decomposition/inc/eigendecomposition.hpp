#ifndef LINALG_EIGENDECOMPOSITION_HPP
#define LINALG_EIGENDECOMPOSITION_HPP

#include "../../utilities/inc/linalgutilities.hpp"
#include "../../overloads/inc/matrixoperators.hpp"

#include <utility>

namespace linalg {

Vector principalComponent(const Matrix& matrix, double tolerance=1e-10, size_t maxIterations=100);

}

#endif