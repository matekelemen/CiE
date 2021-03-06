#ifndef CIE_LINALG_MATRIX_IMPL_HPP
#define CIE_LINALG_MATRIX_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"

// --- STL Includes ---
#include <string>

namespace cie::linalg {


template <concepts::NumericType ValueType>
Matrix<ValueType>::Matrix( Size size1, Size size2, ValueType value ) :
    size1_( size1 ), 
    size2_( size2 ), 
    data_( size1 * size2, value ),
    _transpose(false)
{
}


template <concepts::NumericType ValueType>
Matrix<ValueType>::Matrix(Size size1, Size size2) :
	Matrix(size1, size2, 0.0)
{
}


template <concepts::NumericType ValueType>
Matrix<ValueType>::Matrix() :
	Matrix(0, 0)
{
}


template <concepts::NumericType ValueType>
Matrix<ValueType>::Matrix( const std::vector<ValueType>& rowMajorData, Size size1 ) :
    size1_( size1 ), 
    size2_( 0 ), 
    data_( rowMajorData ),
    _transpose(false)
{
    if( size1 != 0 )
        size2_ = rowMajorData.size( ) / size1;
    else
    {
        CIE_OUT_OF_RANGE_CHECK(  rowMajorData.size()==0 )
    }

    CIE_OUT_OF_RANGE_CHECK( size1_ * size2_ == rowMajorData.size( ) )
}


template <concepts::NumericType ValueType>
Matrix<ValueType>::Matrix( const std::vector<DoubleVector>& vectorOfRows ) :
    size1_( vectorOfRows.size( ) ), 
    size2_( 0 ),
    _transpose(false)
{
    if( size1_ != 0 )
    {
        size2_ = vectorOfRows.front( ).size( );
        data_.resize( size1_ * size2_ );

        for( Size i = 0; i < size1_; ++i )
        {
            CIE_OUT_OF_RANGE_CHECK(  vectorOfRows[i].size( ) == size2_ )
            std::copy( vectorOfRows[i].begin( ), vectorOfRows[i].end( ), data_.begin( ) + ( i * size2_ ) );
        }
    }
}


template <concepts::NumericType ValueType>
inline ValueType& Matrix<ValueType>::operator()( Size i, Size j )
{
    checkIndices(i,j);

    if (!_transpose)
        return data_[i * size2_ + j];
    else
        return data_[j*size2_ + i];
}


template <concepts::NumericType ValueType>
inline ValueType Matrix<ValueType>::operator()( Size i, Size j ) const
{
    checkIndices(i, j);

    if (!_transpose)
        return data_[i * size2_ + j];
    else
        return data_[j*size2_ + i];
}


template <concepts::NumericType ValueType>
inline ValueType& Matrix<ValueType>::operator[]( Size i )
{
    return data_[i];
}


template <concepts::NumericType ValueType>
inline ValueType Matrix<ValueType>::operator[]( Size i ) const
{
    return data_[i];
}


template <concepts::NumericType ValueType>
inline Size Matrix<ValueType>::size1( ) const
{
    return size1_;
}


template <concepts::NumericType ValueType>
inline Size Matrix<ValueType>::size2( ) const
{
    return size2_;
}


template <concepts::NumericType ValueType>
inline Size Matrix<ValueType>::size() const
{
    return data_.size();
}


template <concepts::NumericType ValueType>
inline std::array<Size, 2> Matrix<ValueType>::sizes( ) const
{
    return { size1_, size2_ };
}


template <concepts::NumericType ValueType>
inline typename Matrix<ValueType>::iterator
Matrix<ValueType>::begin()
{
    return this->data_.begin();
}


template <concepts::NumericType ValueType>
inline typename Matrix<ValueType>::const_iterator
Matrix<ValueType>::begin() const
{
    return this->data_.begin();
}


template <concepts::NumericType ValueType>
inline typename Matrix<ValueType>::iterator
Matrix<ValueType>::end()
{
    return this->data_.end();
}


template <concepts::NumericType ValueType>
inline typename Matrix<ValueType>::const_iterator
Matrix<ValueType>::end() const
{
    return this->data_.end();
}


template <concepts::NumericType ValueType>
inline void Matrix<ValueType>::transpose()
{
    Size temp = size1_;
    size1_      = size2_;
    size2_      = temp;
    _transpose  = !_transpose;
}


template <concepts::NumericType ValueType>
inline void Matrix<ValueType>::checkIndices(Size i, Size j) const
{
    CIE_OUT_OF_RANGE_CHECK(  i < size1_ )
    CIE_OUT_OF_RANGE_CHECK(  j < size2_ )
}

} // namespace cie::linalg

#endif