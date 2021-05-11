#ifndef CIE_CSG_TRANSFORM_HPP
#define CIE_CSG_TRANSFORM_HPP

// --- LinAlg Includes ---
#include "linalg/packages/spatial/inc/SpatialTransform.hpp"

// --- Internal Includes ---
#include "CSG/packages/operators/inc/UnaryOperator.hpp"


namespace cie::csg {


template < Size Dimension,
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
requires (Dimension==2 || Dimension==3)
class Transform :
    public UnaryOperator<Dimension,ValueType,CoordinateType>,
    public linalg::SpatialTransform<CoordinateType>
{
public:
    using typename UnaryOperator<Dimension,ValueType,CoordinateType>::point_type;
    using typename UnaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr;

    using transform_type = linalg::SpatialTransform<CoordinateType>;

public:
    template <class ...Args>
    Transform( operand_ptr p_operand, Args&&... r_args );
    
    Transform() = default;

    Transform( Transform<Dimension,ValueType,CoordinateType>&& r_rhs ) = default;

    Transform( const Transform<Dimension,ValueType,CoordinateType>& r_rhs ) = default;

    Transform<Dimension,ValueType,CoordinateType>& operator=( const Transform<Dimension,ValueType,CoordinateType>& r_rhs ) = default;

    Transform<Dimension,ValueType,CoordinateType>& set( transform_type&& r_transform );

    virtual ValueType at( const point_type& r_point ) const override;
};


} // namespace cie::csg

#include "CSG/packages/operators/impl/Transform_impl.hpp"

#endif