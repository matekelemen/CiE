#ifndef CIE_CSG_CSG_OPERATORS_HPP
#define CIE_CSG_CSG_OPERATORS_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"

namespace cie::csg {


// ---------------------------------------------------------
// ABSTRACT OPERATORS
// ---------------------------------------------------------

template <  Size N, 
            concepts::CopyConstructible ValueType = Bool,
            concepts::NumericType CoordinateType = Double   >
class UnaryOperator : public CSGObject<N,ValueType,CoordinateType>
{
public:
    template <class CSGObjectType>
    CSGObject<N,ValueType,CoordinateType>& bindRhs( CSGObjectType&& rhs )
        requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>;

    template <class CSGObjectType>
    CSGObject<N,ValueType,CoordinateType>& bindRhsPtr( std::shared_ptr<CSGObjectType> rhs )
        requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>;

    template <class CSGObjectType, class ...Args>
    CSGObject<N,ValueType,CoordinateType>& emplaceRhs( Args&&... args )
        requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>;

    std::shared_ptr<typename UnaryOperator<N,ValueType,CoordinateType>::abstract_base_type>& rhs();

protected:
    virtual ValueType at(const typename UnaryOperator<N, ValueType, CoordinateType>::point_type& point) const override = 0;

    std::shared_ptr<typename UnaryOperator<N, ValueType, CoordinateType>::abstract_base_type> _rhs;
};




template <  Size N, 
            concepts::CopyConstructible ValueType = Bool,
            concepts::NumericType CoordinateType = Double   >
class BinaryOperator : public UnaryOperator<N,ValueType,CoordinateType>
{
public:
    template <class CSGObjectType>
    CSGObject<N,ValueType,CoordinateType>& bindLhs( CSGObjectType&& lhs )
        requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>;

    template <class CSGObjectType>
    CSGObject<N,ValueType,CoordinateType>& bindLhsPtr( std::shared_ptr<CSGObjectType> lhs )
        requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>;

    template <class CSGObjectType, class ...Args>
    CSGObject<N,ValueType,CoordinateType>& emplaceLhs( Args&&... args )
        requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>;

    std::shared_ptr<typename BinaryOperator<N, ValueType, CoordinateType>::abstract_base_type>& lhs();

protected:
    virtual ValueType at(const typename BinaryOperator<N, ValueType, CoordinateType>::point_type& point) const override = 0;

    std::shared_ptr<typename BinaryOperator<N, ValueType, CoordinateType>::abstract_base_type> _lhs;
};


// ---------------------------------------------------------
// RIGID BODY OPERATORS
// ---------------------------------------------------------
// TODO

// ---------------------------------------------------------
// BOOLEAN OPERATORS
// ---------------------------------------------------------
namespace boolean {

template <Size N, concepts::NumericType CoordinateType = Double>
class Complement : public UnaryOperator<N,Bool,CoordinateType>
{
protected:
    virtual Bool at(const typename Complement<N,CoordinateType>::point_type& point) const override;
};


template <Size N, concepts::NumericType CoordinateType = Double>
class Union : public BinaryOperator<N,Bool,CoordinateType>
{
protected:
    virtual Bool at(const typename Union<N, CoordinateType>::point_type& point) const override;
};


template <Size N, concepts::NumericType CoordinateType = Double>
class Subtraction : public BinaryOperator<N,Bool,CoordinateType>
{
protected:
    virtual Bool at(const typename Subtraction<N, CoordinateType>::point_type& point) const override;
};


template <Size N, concepts::NumericType CoordinateType = Double>
class Intersection : public BinaryOperator<N,Bool,CoordinateType>
{
protected:
    virtual Bool at(const typename Intersection<N, CoordinateType>::point_type& point) const override;
};


} // namespace boolean


} // namespace cie::csg

#include "CSG/packages/primitives/impl/csgoperators_impl.hpp"

#endif