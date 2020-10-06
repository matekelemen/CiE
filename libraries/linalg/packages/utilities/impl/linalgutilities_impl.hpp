#ifndef CIE_LINALG_UTILITIES_IMPL_HPP
#define CIE_LINALG_UTILITIES_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>
#include <cmath>
#include <numeric>


namespace cie::linalg {

template <class ValueType>
void updatePermutation( const Matrix<ValueType>& matrix,
                        PermutationVector& permutation,
                        Size index,
                        double singularTolerance )
{
    auto compare = [&]( Size i1, Size i2 )
    {
        return std::abs( matrix( permutation[i1], index ) ) <
               std::abs( matrix( permutation[i2], index ) );
    };

    // Search for max element in column starting at (index, index)
    auto pivot = std::max_element( permutation.begin( ) + index, permutation.end( ), compare );

    if (std::abs( matrix( *pivot, index ) ) < singularTolerance)
        CIE_THROW( MatrixError, "Matrix is singular!" )

    // Swap indices in permutation vector
    std::iter_swap( permutation.begin( ) + index, pivot );
}

} // namespace cie::linalg



#endif