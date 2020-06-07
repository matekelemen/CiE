#ifndef LINALG_EIGENDECOMPOSITION_HPP
#define LINALG_EIGENDECOMPOSITION_HPP

// --- Utility Includes ---
#include <cieutils/types.hpp>

// --- Internal Includes ---
#include "../../types/inc/vectortypes.hpp"
#include "../../types/inc/matrix.hpp"

// --- STD Includes ---
#include <utility>


namespace cie {
namespace linalg {

DoubleVector principalComponent(const Matrix<Double>& matrix, Double tolerance=1e-10, Size maxIterations=100);

}
}

#endif