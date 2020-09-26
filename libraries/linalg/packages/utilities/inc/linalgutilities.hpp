#ifndef CIE_LINALG_UTILITIES_HPP
#define CIE_LINALG_UTILITIES_HPP

// --- Utility Includes ---
#include <cieutils/types.hpp>

// --- Internal includes ---
#include "linalg/packages/types/inc/matrix.hpp"
#include "linalg/packages/utilities/inc/errortypes.hpp"

namespace cie::linalg {


using PermutationVector = std::vector<Size>;

template <class ValueType>
void updatePermutation( const Matrix<ValueType>& matrix,
                        PermutationVector& permutation,
                        Size index,
                        double singularTolerance );

void runtime_check( bool result, const char message[] );


} // namespace cie::linalg

#include "linalg/packages/utilities/impl/linalgutilities_impl.hpp"

#endif