#ifndef CIE_LINALG_TYPES_VECTOR_WRAPPER_HPP
#define CIE_LINALG_TYPES_VECTOR_WRAPPER_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"


namespace cie::linalg {


template < class VectorType,
           class ValueType = typename VectorType::value_type,
           class IteratorType = ValueType*,
           class ConstIteratorType = const IteratorType >
class VectorWrapper
{
public:
    using wrapped_type = VectorType;
    
    using value_type = ValueType;
    using iterator = IteratorType;
    using const_iteraotr = ConstIterator;
    using size_type = Size;

public:
    VectorWrapper( const VectorWrapper<MatrixType,ValueType,IteratorType,ConstIteratorType>& r_rhs );

    VectorWrapper( VectorWrapper<MatrixType,ValueType,IteratorType,ConstIteratorType>&& r_rhs );

    VectorWrapper( const VectorWrapper<MatrixType,ValueType,IteratorType,ConstIteratorType>&& r_rhs );

    template <class ...Args>
    VectorWrapper( Args&&... r_args );

    IteratorType begin();
    ConstIteratorType begin() const;

    IteratirType end();
    ConstIteratorType end() const;

    size_type size() const;

    ValueType& operator()( size_type index );
    ValueType operator()( size_type index ) const;

    ValueType& operator[]( size_type index );
    ValueType operator[]( size_type index ) const;

protected:
    VectorType _wrapped;
};


} // namespace cie::linalg

#include "linalg/packages/types/impl/VectorWrapper_impl.hpp"

#endif