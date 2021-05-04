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
    using const_iterator = ConstIteratorType;
    using size_type = Size;

public:
    template <class ...Args>
    VectorWrapper( Args&&... r_args );

    VectorWrapper( const VectorWrapper<VectorType,ValueType,IteratorType,ConstIteratorType>& r_rhs ) = default;

    VectorWrapper( VectorWrapper<VectorType,ValueType,IteratorType,ConstIteratorType>&& r_rhs ) = default;

    VectorWrapper<VectorType,ValueType,IteratorType,ConstIteratorType>& operator=( const VectorWrapper<VectorType,ValueType,IteratorType,ConstIteratorType>& r_rhs ) = default;

    size_type size() const;

    void resize( Size newSize );

    IteratorType begin();
    ConstIteratorType begin() const;

    IteratorType end();
    ConstIteratorType end() const;

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