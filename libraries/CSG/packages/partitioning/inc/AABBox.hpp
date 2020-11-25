#ifndef CIE_CSG_PARTITIONING_AABBOX_HPP
#define CIE_CSG_PARTITIONING_AABBOX_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Box.hpp"


namespace cie::csg {


template < Size Dimension,
           concepts::NumericType CoordinateType = Double >
class AABBox : public boolean::Box<Dimension,CoordinateType>
{
public:
    explicit AABBox();
    AABBox( const typename AABBox<Dimension,CoordinateType>::point_type& r_base,
            const typename AABBox<Dimension,CoordinateType>::point_type& r_lengths );

    /**
     * Check whether this box fully encloses the argument box:
     * all points of the argument must lie inside
     * 
     * Note: no overlaps permitted
     */
    Bool contains( const AABBox<Dimension,CoordinateType>& r_box ) const;

    /**
     * Check for intersections with the argument box:
     * argument must have at least one point inside and one outside
     * 
     * Note: touching does not qualify as intersection
     */
    Bool intersects( const AABBox<Dimension,CoordinateType>& r_box ) const;
};


} // namespace cie::csg

#include "CSG/packages/partitioning/impl/AABBox_impl.hpp"

#endif