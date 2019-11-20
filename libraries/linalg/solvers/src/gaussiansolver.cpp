#include "../inc/gaussiansolver.hpp"

#include "../../types/inc/typeoperations.hpp"

namespace cie {
namespace linalg {


DoubleVector solve( const Matrix& matrix,
              const DoubleVector& rhs )
{
    size_t n = matrix.size1( );

    if( n == 0 )
    {
        return { };
    }

    runtime_check( matrix.size2( ) == n, "Solve needs a square matrix!" );
    runtime_check( rhs.size( ) == n, "Matrix and vector sizes don't match!" );

    // Instead of swapping rows in our matrix we use a permutation vector
    PermutationVector permute( n );

    std::iota( permute.begin( ), permute.end( ), 0 ); // initialize  with 1 ... n

    DoubleVector x( n ), tmpB = rhs;
    Matrix tmpM = matrix;

    // Create two convenience lambda functions to access and permute tmpM and tmpP
    auto permuteMatrix = [&]( size_t i, size_t j ) -> double& { return tmpM( permute[i], j ); };
    auto permuteDoubleVector = [&]( size_t i ) -> double& { return tmpB[permute[i]]; };

    double tolerance = 1e-10 * norm<Matrix>( matrix );

    // LU decomposition with partial pivoting
    for( size_t k = 0; k < n - 1; k++ )
    {
        updatePermutation( tmpM, permute, k, tolerance );

        for( size_t i = k + 1; i < n; i++ )
        {
            permuteMatrix( i, k ) /= permuteMatrix( k, k );

            for( size_t j = k + 1; j < n; j++)
            {
                permuteMatrix( i, j ) -= permuteMatrix( i, k ) * permuteMatrix( k, j );
            }
        }
    }

    // Just for the singularity check
    updatePermutation( tmpM, permute, n - 1, tolerance );

    // forward substitution
    for( size_t i = 1; i < n; i++ )
    {
        for( size_t j = 0; j < i; j++ )
        {
            permuteDoubleVector( i ) -= permuteMatrix( i, j ) * permuteDoubleVector( j );
        }
    }

    // backward substitution
    for( size_t i = n; i > 0; i-- )
    {
        for( size_t j = i; j < n; j++ )
        {
            permuteDoubleVector( i - 1 ) -= permuteMatrix( i - 1, j ) * permuteDoubleVector( j );
        }

        permuteDoubleVector( i - 1 ) /= permuteMatrix( i - 1, i - 1 );
    }

    // permute solution and write into x
    std::transform( permute.begin( ), permute.end( ), x.begin( ), [&]( size_t i ){ return tmpB[i]; } );

    return x;
}

}
}