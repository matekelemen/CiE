#ifndef CIE_CSG_CSG_OPERATORS_IMPL_HPP
#define CIE_CSG_CSG_OPERATORS_IMPL_HPP

namespace cie {
namespace csg {


// ---------------------------------------------------------
// ABSTRACT OPERATORS
// ---------------------------------------------------------
template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
template <class CSGObjectType>
CSGObject<N,ValueType,CoordinateType>&
UnaryOperator<N,ValueType,CoordinateType>::bindRhs( CSGObjectType&& rhs )
    requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>
{
    _rhs = std::make_shared<CSGObjectType>( std::move(rhs) );
    return *this;
}


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
template <class CSGObjectType>
CSGObject<N,ValueType,CoordinateType>&
UnaryOperator<N,ValueType,CoordinateType>::bindRhsPtr( std::shared_ptr<CSGObjectType> rhs )
    requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>
{
    _rhs = rhs;
    return *this;
}


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
template <class CSGObjectType, class ...Args>
CSGObject<N,ValueType,CoordinateType>&
UnaryOperator<N,ValueType,CoordinateType>::emplaceRhs( Args&&... args )
    requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>
{
    _rhs = std::shared_ptr<typename UnaryOperator<N,ValueType,CoordinateType>::abstract_base_type>( 
        new CSGObjectType(std::forward<Args>(args)...) );
    return *this;
}


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
std::shared_ptr<typename UnaryOperator<N,ValueType,CoordinateType>::abstract_base_type>&
UnaryOperator<N,ValueType,CoordinateType>::rhs()
{
    return _rhs;
}


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
template <class CSGObjectType>
CSGObject<N,ValueType,CoordinateType>&
BinaryOperator<N,ValueType,CoordinateType>::bindLhs( CSGObjectType&& lhs )
    requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>
{
    _lhs = std::make_shared<CSGObjectType>( std::move(lhs) );
    return *this;
}


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
template <class CSGObjectType>
CSGObject<N,ValueType,CoordinateType>&
BinaryOperator<N,ValueType,CoordinateType>::bindLhsPtr( std::shared_ptr<CSGObjectType> lhs )
    requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>
{
    _lhs = lhs;
    return *this;
}


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
template <class CSGObjectType, class ...Args>
CSGObject<N,ValueType,CoordinateType>&
BinaryOperator<N,ValueType,CoordinateType>::emplaceLhs( Args&&... args )
    requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>
{
    _lhs = std::shared_ptr<typename BinaryOperator<N,ValueType,CoordinateType>::abstract_base_type>( 
        new CSGObjectType(std::forward<Args>(args)...) );
    return *this;
}


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
std::shared_ptr<typename BinaryOperator<N,ValueType,CoordinateType>::abstract_base_type>&
BinaryOperator<N,ValueType,CoordinateType>::lhs()
{
    return _lhs;
}


// ---------------------------------------------------------
// BOOLEAN OPERATORS
// ---------------------------------------------------------
template <Size N, concepts::NumericType CoordinateType>
Bool BooleanComplement<N,CoordinateType>::operator()( const typename BooleanComplement<N,CoordinateType>::point_type& point ) const
{
    return !this->_rhs->evaluate(point);
}


template <Size N, concepts::NumericType CoordinateType>
Bool BooleanUnion<N,CoordinateType>::operator()( const typename BooleanUnion<N,CoordinateType>::point_type& point ) const
{
    return this->_lhs->evaluate(point) || this->_rhs->evaluate(point);
}


template <Size N, concepts::NumericType CoordinateType>
Bool BooleanSubtraction<N,CoordinateType>::operator()( const typename BooleanSubtraction<N,CoordinateType>::point_type& point ) const
{
    return this->_lhs->evaluate(point) && !this->_rhs->evaluate(point);
}


template <Size N, concepts::NumericType CoordinateType>
Bool BooleanIntersection<N,CoordinateType>::operator()( const typename BooleanIntersection<N,CoordinateType>::point_type& point ) const
{
    return this->_lhs->evaluate(point) && this->_rhs->evaluate(point);
}


}
}

#endif