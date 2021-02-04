#ifndef CIE_CSG_PRIMITIVES_ELLIPSOID_HPP
#define CIE_CSG_PRIMITIVES_ELLIPSOID_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Primitive.hpp"
#include "CSG/packages/primitives/inc/csgobject.hpp"


namespace cie::csg {


/// Ellipsoid template
template < Size Dimension,
           concepts::NumericType CoordinateType = Double >
class Ellipsoid : public AbsPrimitive<Dimension,CoordinateType>
{
public:
    using typename AbsPrimitive<Dimension,CoordinateType>::point_type;
public:
    Ellipsoid( const point_type& r_center,
               const point_type& r_radii );

    Ellipsoid();

    Ellipsoid( const Ellipsoid<Dimension,CoordinateType>& r_rhs ) = default;
    Ellipsoid( Ellipsoid<Dimension,CoordinateType>&& r_rhs ) = default;
    Ellipsoid<Dimension,CoordinateType>& operator=( const Ellipsoid<Dimension,CoordinateType>& r_rhs ) = default;

    virtual Bool isDegenerate() const override;

    point_type& center();
    const point_type& center() const;

    point_type& radii();
    const point_type& radii() const;

protected:
    point_type  _center;
    point_type  _radii;
};




namespace boolean {


/// Ellipsoid with point membership test
template < Size Dimension,
           concepts::NumericType CoordinateType = Double >
class Ellipsoid :
    public cie::csg::Ellipsoid<Dimension,CoordinateType>,
    public CSGObject<Dimension,Bool,CoordinateType>
{
public:
    using typename cie::csg::Ellipsoid<Dimension,CoordinateType>::point_type;
public:
    Ellipsoid( const point_type& r_center,
               const point_type& r_radii );

    Ellipsoid();

    Ellipsoid( const Ellipsoid<Dimension,CoordinateType>& r_rhs ) = default;
    Ellipsoid( Ellipsoid<Dimension,CoordinateType>&& r_rhs ) = default;
    Ellipsoid<Dimension,CoordinateType>& operator=( const Ellipsoid<Dimension,CoordinateType>& r_rhs ) = default;

protected:
    virtual Bool at( const point_type& r_point ) const override;
};


} // namespace boolean


} // namespace cie::csg

#include "CSG/packages/primitives/impl/Ellipsoid_impl.hpp"

#endif