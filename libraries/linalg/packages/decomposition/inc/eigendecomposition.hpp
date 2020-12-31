#ifndef LINALG_EIGENDECOMPOSITION_HPP
#define LINALG_EIGENDECOMPOSITION_HPP

// --- Utility Includes ---
#include <cieutils/types.hpp>

// --- Internal Includes ---
#include "linalg/packages/types/inc/vectortypes.hpp"
#include "linalg/packages/types/inc/matrix.hpp"

// --- STL Includes ---
#include <utility>


namespace cie::linalg {

DoubleVector principalComponent(const Matrix<Double>& matrix, Double tolerance=1e-10, Size maxIterations=100);

} // namespace cie::linalg

#endif