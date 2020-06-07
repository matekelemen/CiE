#ifndef CIE_LINALG_UTILITIES_HPP
#define CIE_LINALG_UTILITIES_HPP

// --- Utility Includes ---
#include <cieutils/types.hpp>

// --- Internal includes ---
#include "../../types/inc/matrix.hpp"
#include "errortypes.hpp"

namespace cie {
namespace linalg {


using PermutationVector = std::vector<Size>;

template <class ValueType>
void updatePermutation( const Matrix<ValueType>& matrix,
                        PermutationVector& permutation,
                        Size index,
                        double singularTolerance );

void runtime_check( bool result, const char message[] );


} // namespace linalg
}

#include "linalgutilities_impl.hpp"

#endif