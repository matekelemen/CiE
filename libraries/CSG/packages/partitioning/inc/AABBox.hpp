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

    AABBox( const AABBox<Dimension,CoordinateType>& r_rhs ) = default;
    AABBox<Dimension,CoordinateType>& operator=( const AABBox<Dimension,CoordinateType>& r_rhs ) = default;

    /**
     * Check whether this box fully encloses the argument box:
     * all points of the argument must lie inside
     * 
     * \param r_box query box
     * 
     * \note closed boundaries
     */
    Bool contains( const AABBox<Dimension,CoordinateType>& r_box ) const;

    /**
     * Check for intersections with the argument box:
     * argument must have at least one point inside and one outside
     * 
     * \param r_box query box
     * 
     * \note touching does not qualify as intersection
     */
    Bool intersects( const AABBox<Dimension,CoordinateType>& r_box ) const;

    /**
     * Expand box to include target box
     * 
     * \param r_box target box
     * 
     * \note target box will not necessarily be "contained"
     */
    void include( const AABBox<Dimension,CoordinateType>& r_box );
};


} // namespace cie::csg

#include "CSG/packages/partitioning/impl/AABBox_impl.hpp"

#endif