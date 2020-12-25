#ifndef CIE_CSG_SPHERE_HPP
#define CIE_CSG_SPHERE_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Primitive.hpp"


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType = Double>
class Sphere : public AbsPrimitive<Dimension,CoordinateType>
{
public:
    Sphere( const typename Sphere<Dimension,CoordinateType>::point_type& r_center,
            CoordinateType radius );

    template <class ContainerType>
    requires concepts::ClassContainer<ContainerType,CoordinateType>
    Sphere( const ContainerType& r_center,
            CoordinateType radius );

    virtual Bool isDegenerate() const override;

    typename Sphere<Dimension,CoordinateType>::point_type& center();
    const typename Sphere<Dimension,CoordinateType>::point_type& center() const;
    CoordinateType& radius();
    const CoordinateType& radius() const;

protected:
    typename Sphere<Dimension,CoordinateType>::point_type _center;
    CoordinateType                                        _radius;
};



/* --- BOOLEAN SPHERE --- */

namespace boolean {


template <Size Dimension, concepts::NumericType CoordinateType>
class Sphere :
    public cie::csg::Sphere<Dimension,CoordinateType>,
    public CSGObject<Dimension,Bool,CoordinateType>
{
public:
    Sphere( const typename Sphere<Dimension,CoordinateType>::point_type& r_center,
            CoordinateType radius );

    template <class ContainerType>
    requires concepts::ClassContainer<ContainerType,CoordinateType>
    Sphere( const ContainerType& r_center,
            CoordinateType radius );

    virtual Bool at( const typename Sphere<Dimension,CoordinateType>::point_type& r_point ) const override;
};


} // namespace boolean

} // namespace cie::csg

#include "CSG/packages/primitives/impl/Sphere_impl.hpp"

#endif