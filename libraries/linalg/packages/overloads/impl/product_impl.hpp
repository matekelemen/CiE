#ifndef CIE_LINALG_PRODUCT_IMPL_HPP
#define CIE_LINALG_PRODUCT_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"


namespace cie::linalg {


template <class ArrayType, concepts::NumericType NT>
void scale( VectorWrapper<ArrayType>& r_vector, NT coefficient )
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( auto& r_component : r_vector )
        r_component *= coefficient;

    CIE_END_EXCEPTION_TRACING
}


template <class MatrixType, concepts::NumericType NT>
void scale( MatrixWrapper<MatrixType>& r_matrix, NT coefficient )
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( auto& r_component : r_matrix )
        r_component *= coefficient;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericContainer ArrayType>
typename ArrayType::value_type dotProduct( const VectorWrapper<ArrayType>& r_lhs, const VectorWrapper<ArrayType>& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_OUT_OF_RANGE_CHECK( r_lhs.size() == r_rhs.size() );

    typename ArrayType::value_type output = 0;

    for ( Size i=0; i<r_lhs.size(); ++i )
        output += r_lhs(i) * r_rhs(i);

    CIE_END_EXCEPTION_TRACING
}


template <class MatrixType, class ArrayType>
VectorWrapper<ArrayType> dotProduct( const MatrixWrapper<MatrixType>& r_lhs, const VectorWrapper<ArrayType>& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_OUT_OF_RANGE_CHECK( r_lhs.numberOfColumns() == r_rhs.size() )

    VectorWrapper<ArrayType> output;
    output.resize( r_lhs.numberOfRows() );
    CIE_OUT_OF_RANGE_CHECK( output.size() == r_lhs.numberOfRows() )

    for ( Size rowIndex=0; rowIndex<r_lhs.numberOfRows(); ++rowIndex )
    {
        auto& r_component = output[rowIndex];
        r_component = 0;

        for ( Size columnIndex=0; columnIndex<r_lhs.numberOfColumns(); ++columnIndex )
            r_component += r_lhs(rowIndex, columnIndex) * r_rhs(columnIndex);
    }

    return output;

    CIE_END_EXCEPTION_TRACING
}


template <class ArrayType, class MatrixType>
VectorWrapper<ArrayType> dotProduct( const VectorWrapper<ArrayType>& r_lhs, const MatrixWrapper<MatrixType>& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_OUT_OF_RANGE_CHECK( r_rhs.numberOfRows() == r_lhs.size() )

    VectorWrapper<ArrayType> output;
    output.resize( r_rhs.numberOfColumns() );
    CIE_OUT_OF_RANGE_CHECK( output.size() == r_rhs.numberOfColumns() )

    for ( Size columnIndex=0; columnIndex<r_rhs.numberOfColumns(); ++columnIndex )
    {
        auto& r_component = output[columnIndex];
        r_component = 0;

        for ( Size rowIndex=0; rowIndex<r_rhs.numberOfRows(); ++rowIndex )
            r_component += r_lhs(rowIndex) * r_rhs(rowIndex, columnIndex);
    }

    return output;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::linalg


#endif