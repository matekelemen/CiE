#include "matrix.hpp"

namespace linalg {

Matrix::Matrix( size_t size1, size_t size2, double value ) :
    size1_( size1 ), size2_( size2 ), data_( size1 * size2, value )
{
}

Matrix::Matrix(size_t size1, size_t size2) :
	Matrix(size1, size2, 0.0)
{
}

Matrix::Matrix() :
	Matrix(0, 0)
{
}

Matrix::Matrix( const std::vector<double>& rowMajorData, size_t size1 ) :
    size1_( size1 ), size2_( 0 ), data_( rowMajorData )
{
    if( size1 != 0 )
    {
        size2_ = rowMajorData.size( ) / size1;
    }
    else
    {
        if (rowMajorData.size()!=0)
            throw std::runtime_error("Zero size with non-zero sized data.");
    }

    if (size1_ * size2_ != rowMajorData.size( ))
            throw std::runtime_error("Inconsistent sizes!");
}

Matrix::Matrix( const std::vector<Vector>& vectorOfRows ) :
    size1_( vectorOfRows.size( ) ), size2_( 0 )
{
    if( size1_ != 0 )
    {
        size2_ = vectorOfRows.front( ).size( );
        data_.resize( size1_ * size2_ );

        for( size_t i = 0; i < size1_; ++i )
        {
            if (vectorOfRows[i].size( ) != size2_)
                throw std::runtime_error("Inconsistent input data!");

            std::copy( vectorOfRows[i].begin( ), vectorOfRows[i].end( ), data_.begin( ) + ( i * size2_ ) );
        }
    }
}

}