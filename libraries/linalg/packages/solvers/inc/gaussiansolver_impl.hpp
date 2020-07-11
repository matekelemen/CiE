#ifndef CIE_LINALG_GAUSSIAN_SOLVER_IMPL_HPP
#define CIE_LINALG_GAUSSIAN_SOLVER_IMPL_HPP

// --- Internal Includes ---
#include "../../types/inc/typeoperations.hpp"

// --- STL Includes ---
#include <cmath>
#include <numeric>
#include <algorithm>

namespace cie {
namespace linalg {


template <class ValueType, concepts::NumericContainer ContainerType>
ContainerType solve(    const Matrix<ValueType>& matrix,
                        const ContainerType& vector )
{
    Size n = matrix.size1( );

    if( n == 0 )
    {
        return { };
    }

    runtime_check( matrix.size2( ) == n, "Solve needs a square matrix!" );
    runtime_check( vector.size( ) == n, "Matrix and vector sizes don't match!" );

    // Instead of swapping rows in our matrix we use a permutation vector
    PermutationVector permute( n );

    std::iota( permute.begin( ), permute.end( ), 0 ); // initialize  with 1 ... n

    DoubleVector x( n ), tmpB = vector;
    Matrix<ValueType> tmpM = matrix;

    // Create two convenience lambda functions to access and permute tmpM and tmpP
    auto permuteMatrix = [&]( Size i, Size j ) -> ValueType& { return tmpM( permute[i], j ); };
    auto permuteDoubleVector = [&]( Size i ) -> ValueType& { return tmpB[permute[i]]; };

    ValueType tolerance = 1e-10 * norm<Matrix<ValueType>>( matrix );

    // LU decomposition with partial pivoting
    for( Size k = 0; k < n - 1; k++ )
    {
        updatePermutation( tmpM, permute, k, tolerance );

        for( Size i = k + 1; i < n; i++ )
        {
            permuteMatrix( i, k ) /= permuteMatrix( k, k );

            for( Size j = k + 1; j < n; j++)
            {
                permuteMatrix( i, j ) -= permuteMatrix( i, k ) * permuteMatrix( k, j );
            }
        }
    }

    // Just for the singularity check
    updatePermutation( tmpM, permute, n - 1, tolerance );

    // forward substitution
    for( Size i = 1; i < n; i++ )
    {
        for( Size j = 0; j < i; j++ )
        {
            permuteDoubleVector( i ) -= permuteMatrix( i, j ) * permuteDoubleVector( j );
        }
    }

    // backward substitution
    for( Size i = n; i > 0; i-- )
    {
        for( Size j = i; j < n; j++ )
        {
            permuteDoubleVector( i - 1 ) -= permuteMatrix( i - 1, j ) * permuteDoubleVector( j );
        }

        permuteDoubleVector( i - 1 ) /= permuteMatrix( i - 1, i - 1 );
    }

    // permute solution and write into x
    std::transform( permute.begin( ), permute.end( ), x.begin( ), [&]( Size i ){ return tmpB[i]; } );

    return x;
}

}
}

#endif