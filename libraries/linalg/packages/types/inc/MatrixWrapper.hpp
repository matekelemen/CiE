#ifndef CIE_LINALG_TYPES_MATRIX_WRAPPER_HPP
#define CIE_LINALG_TYPES_MATRIX_WRAPPER_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"


namespace cie::linalg {


template < class MatrixType,
           class ValueType = typename MatrixType::value_type,
           class IteratorType = ValueType*,
           class ConstIteratorType = const IteratorType >
class MatrixWrapper
{
public:
    using matrix_type = MatrixType;

    using value_type = ValueType;
    using iterator = IteratorType;
    using const_iterator = ConstIteratorType;
    using size_type = Size;

public:
    MatrixWrapper( const MatrixWrapper<MatrixType,ValueType,IteratorType,ConstIteratorType>& r_rhs );

    MatrixWrapper( MatrixWrapper<MatrixType,ValueType,IteratorType,ConstIteratorType>&& r_rhs );

    MatrixWrapper( const MatrixWrapper<MatrixType,ValueType,IteratorType,ConstIteratorType>&& r_rhs );

    template <class ...Args>
    MatrixWrapper( Args&&... r_args );

    MatrixWrapper<MatrixType,ValueType,IteratorType,ConstIteratorType>& operator=( const MatrixWrapper<MatrixType,ValueType,IteratorType,ConstIteratorType>& r_rhs ) = default;

    IteratorType begin();
    ConstIteratorType begin() const;

    IteratorType end();
    ConstIteratorType end() const;

    size_type size() const;
    size_type numberOfRows() const;
    size_type numberOfColumns() const;

    ValueType& operator()( size_type rowIndex, size_type columnIndex );
    ValueType operator()( size_type rowIndex, size_type columnIndex ) const;

public:
    operator const MatrixType&() const;

protected:
    MatrixType _wrapped;
};


} // namespace cie::linalg

#include "linalg/packages/types/impl/MatrixWrapper_impl.hpp"

#endif