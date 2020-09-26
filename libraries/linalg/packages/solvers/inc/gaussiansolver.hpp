#ifndef CIE_LINALG_GAUSSIAN_SOLVER_HPP
#define CIE_LINALG_GAUSSIAN_SOLVER_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "linalg/packages/types/inc/matrix.hpp"
#include "linalg/packages/utilities/inc/linalgutilities.hpp"

namespace cie::linalg {

// Solve linear system of equations
template <class ValueType, concepts::NumericContainer ContainerType>
ContainerType solve(    const Matrix<ValueType>& matrix,
                        const ContainerType& vector );

} // namespace cie::linalg

#include "linalg/packages/solvers/impl/gaussiansolver_impl.hpp"

#endif