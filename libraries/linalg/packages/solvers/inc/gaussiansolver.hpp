#ifndef CIE_LINALG_GAUSSIAN_SOLVER_HPP
#define CIE_LINALG_GAUSSIAN_SOLVER_HPP

// --- Utility Includes ---
#include <cieutils/types.hpp>
#include <cieutils/concepts.hpp>

// --- Internal Includes ---
#include "../../types/inc/matrix.hpp"
#include "../../utilities/inc/linalgutilities.hpp"

namespace cie {
namespace linalg {

// Solve linear system of equations
template <class ValueType, concepts::NumericContainer ContainerType>
ContainerType solve(    const Matrix<ValueType>& matrix,
                        const ContainerType& vector );

}
}

#include "../impl/gaussiansolver_impl.hpp"

#endif