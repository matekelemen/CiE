#ifndef LINALG_EIGENDECOMPOSITION_HPP
#define LINALG_EIGENDECOMPOSITION_HPP

#include "../../types/inc/vectortypes.hpp"
#include "../../types/inc/matrix.hpp"

#include <utility>


namespace cie {
namespace linalg {

DoubleVector principalComponent(const Matrix& matrix, double tolerance=1e-10, size_t maxIterations=100);

}
}

#endif