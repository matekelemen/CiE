#ifndef LINALG_UTILITIES_CPP
#define LINALG_UTILITIES_CPP

#include "../inc/linalgutilities.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

namespace cie {
namespace linalg {

/*
double norm( const Vector& vector )
{
    return std::sqrt( std::inner_product( std::begin( vector ),
                                          std::end( vector ),
                                          std::begin( vector ), 0.0 ) );
}


double norm( const Matrix& matrix )
{
    double result = 0.0;

    for( size_t i = 0; i < matrix.size1( ); ++i )
    {
        for( size_t j = 0; j < matrix.size2( ); ++j )
        {
            result += matrix( i, j ) * matrix( i, j );
        }
    }

    return std::sqrt( result );
}


void normalize(Vector& vector)
{
    double magnitude = norm(vector);
    if (abs(magnitude)<1e-15)
        throw std::runtime_error("Cannot normalize zero vector!");
    for (auto it=vector.begin(); it!=vector.end(); ++it)
        (*it) /= magnitude;
}


void normalize(Matrix& matrix)
{
    double magnitude = norm(matrix);
    if (abs(magnitude)<1e-15)
        throw std::runtime_error("Cannot normalize zero matrix!");
    for (size_t i=0; i<matrix.size1(); ++i){
        for (size_t j=0; j<matrix.size2(); ++j){
            matrix(i,j) /= magnitude;
        }
    }
}
*/

void updatePermutation( const Matrix& matrix,
                        PermutationVector& permutation,
                        size_t index,
                        double singularTolerance )
{
    auto compare = [&]( size_t i1, size_t i2 )
    {
        return std::abs( matrix( permutation[i1], index ) ) <
               std::abs( matrix( permutation[i2], index ) );
    };

    // Search for max element in column starting at (index, index)
    auto pivot = std::max_element( permutation.begin( ) + index, permutation.end( ), compare );

    if (std::abs( matrix( *pivot, index ) ) < singularTolerance)
    throw MatrixError("Matrix is singular!",matrix);

    // Swap indices in permutation vector
    std::iter_swap( permutation.begin( ) + index, pivot );
}


void runtime_check( bool result, const char message[] )
{
    if( !result )
    {
        throw std::runtime_error{ message };
    }
}

    
} // namespace linalg
}

#endif