#ifndef LINALG_UTILITIES_CPP
#define LINALG_UTILITIES_CPP

#include "linalgutilities.hpp"

namespace linalg {


void write( const Vector& vector, std::ostream& out )
{
    linalghelper::writeRow( [&]( size_t i ){ return vector[i]; }, vector.size( ), out, 12 );
}


void write( const Matrix& matrix, std::ostream& out )
{
    size_t size1 = matrix.size1( );
    size_t size2 = matrix.size2( );

    for( size_t i = 0; i < size1; ++i )
    {
        linalghelper::writeRow( [&]( size_t j ){ return matrix( i, j ); }, size2, out, 12 );
    }
}


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







namespace linalghelper {


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

    runtime_check( std::abs( matrix( *pivot, index ) ) > singularTolerance,
                                 "Matrix is singular!" );

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

} // namespace linalghelpers
    
} // namespace linalg

#endif