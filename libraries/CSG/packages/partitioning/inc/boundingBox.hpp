#ifndef CIE_CSG_PARTITIONING_BOUNDING_BOX_HPP
#define CIE_CSG_PARTITIONING_BOUNDING_BOX_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/partitioning/inc/AbsBoundableObject.hpp"
#include "CSG/packages/primitives/inc/csgobject.hpp"


namespace cie::csg {


/// Bounding box factory for objects with internal bounding boxes
template <concepts::HasBoundingBox ObjectType>
const typename ObjectType::bounding_box& boundingBox( ObjectType& r_object );


/// Bounding box factory for points
template <concepts::STLArray PointType>
requires std::is_same_v<PointType,typename Traits<PointType>::point_type>
const AABBox<Traits<PointType>::dimension,typename Traits<PointType>::coordinate_type>
boundingBox( const PointType& r_point );


} // namespace cie::csg



/* --- CONCEPT DEFINITIONS --- */

namespace cie::concepts {

template <class T>
concept BoxBoundable
= requires ( T instance )
{
    { csg::boundingBox( instance ) };
};

} // namespace cie::concepts


#include "CSG/packages/partitioning/impl/boundingBox_impl.hpp"

#endif