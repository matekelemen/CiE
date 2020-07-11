#ifndef CIE_CSG_CSG_OPERATORS_HPP
#define CIE_CSG_CSG_OPERATORS_HPP

// --- Internal Includes ---
#include "csgobject.hpp"

namespace cie {
namespace csg {


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

    std::shared_ptr<typename UnaryOperator::abstract_base_type>& rhs();

protected:
    virtual ValueType at(const typename UnaryOperator::point_type& point) const override = 0;

    std::shared_ptr<typename UnaryOperator::abstract_base_type> _rhs;
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

    std::shared_ptr<typename BinaryOperator::abstract_base_type>& lhs();

protected:
    virtual ValueType at(const typename BinaryOperator::point_type& point) const override = 0;

    std::shared_ptr<typename BinaryOperator::abstract_base_type> _lhs;
};


// ---------------------------------------------------------
// RIGID BODY OPERATORS
// ---------------------------------------------------------
// TODO

// ---------------------------------------------------------
// BOOLEAN OPERATORS
// ---------------------------------------------------------
template <Size N, concepts::NumericType CoordinateType = Double>
class BooleanComplement : public UnaryOperator<N,Bool,CoordinateType>
{
protected:
    virtual Bool at(const typename BooleanComplement::point_type& point) const override;
};


template <Size N, concepts::NumericType CoordinateType = Double>
class BooleanUnion : public BinaryOperator<N,Bool,CoordinateType>
{
protected:
    virtual Bool at(const typename BooleanUnion::point_type& point) const override;
};


template <Size N, concepts::NumericType CoordinateType = Double>
class BooleanSubtraction : public BinaryOperator<N,Bool,CoordinateType>
{
protected:
    virtual Bool at(const typename BooleanSubtraction::point_type& point) const override;
};


template <Size N, concepts::NumericType CoordinateType = Double>
class BooleanIntersection : public BinaryOperator<N,Bool,CoordinateType>
{
protected:
    virtual Bool at(const typename BooleanIntersection::point_type& point) const override;
};


}
}

#include "../impl/csgoperators_impl.hpp"

#endif