#ifndef CIE_CSG_PARTITIONING_BOUNDING_BOX_IMPL_HPP
#define CIE_CSG_PARTITIONING_BOUNDING_BOX_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::csg {


template <concepts::HasBoundingBox ObjectType>
inline const typename ObjectType::bounding_box&
boundingBox( ObjectType& r_object )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return r_object.boundingBox();

    CIE_END_EXCEPTION_TRACING
}



template <concepts::STLArray PointType>
requires std::is_same_v<PointType,typename Traits<PointType>::point_type>
inline const AABBox<Traits<PointType>::dimension,typename Traits<PointType>::coordinate_type>
boundingBox( const PointType& r_point )
{
    CIE_BEGIN_EXCEPTION_TRACING

    constexpr const Size Dimension = Traits<PointType>::dimension;
    using CoordinateType = typename Traits<PointType>::coordinate_type;

    return AABBox<Dimension,CoordinateType>(
        r_point,
        detail::makeOrigin<Dimension,CoordinateType>()
    );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg


#endif