#ifndef CIE_CSG_PARTITIONING_ABS_BOUNDABLE_OBJECT_IMPL_HPP
#define CIE_CSG_PARTITIONING_ABS_BOUNDABLE_OBJECT_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::csg {


template < Size Dimension,
           concepts::NumericType CoordinateType >
AbsBoundableObject<Dimension,CoordinateType>::AbsBoundableObject() :
    _recomputeBoundingBox( true ),
    _boundingBox()
{
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline void
AbsBoundableObject<Dimension,CoordinateType>::boundingBoxShouldRecompute()
{
    this->_recomputeBoundingBox = true;
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline const typename AbsBoundableObject<Dimension,CoordinateType>::bounding_box&
AbsBoundableObject<Dimension,CoordinateType>::boundingBox()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( this->_recomputeBoundingBox )
        this->computeBoundingBox();

    return this->_boundingBox;

    CIE_END_EXCEPTION_TRACING
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline void
AbsBoundableObject<Dimension,CoordinateType>::computeBoundingBox()
{
    this->computeBoundingBox_impl( this->_boundingBox );
    this->_recomputeBoundingBox = false;
}


} // namespace cie::csg


#endif