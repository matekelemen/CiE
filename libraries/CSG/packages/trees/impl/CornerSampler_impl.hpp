#ifndef CIE_CSG_TREES_CORNER_SAMPLE_IMPL_HPP
#define CIE_CSG_TREES_CORNER_SAMPLE_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::csg {


template <concepts::Cube PrimitiveType>
CornerSampler<PrimitiveType>::CornerSampler() :
    _p_indexConverter( new CartesianIndexConverter<PrimitiveType::dimension>(2) )
{
}


template <concepts::Cube PrimitiveType>
inline Size
CornerSampler<PrimitiveType>::size() const
{
    return _p_indexConverter->numberOfPoints();
}


template <concepts::Cube PrimitiveType>
typename PrimitiveType::point_type
CornerSampler<PrimitiveType>::getSamplePoint( const PrimitiveType& r_primitive,
                                              const Size index ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto point = r_primitive.base();
    auto it_pointEnd = point.end();

    auto it_index = this->_p_indexConverter->convert( index ).begin();

    for ( auto it_point=point.begin(); it_point!=it_pointEnd; ++it_point,++it_index )
        *it_point += (*it_index) * r_primitive.length();

    return point;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::Box PrimitiveType>
CornerSampler<PrimitiveType>::CornerSampler() :
    _p_indexConverter( new CartesianIndexConverter<PrimitiveType::dimension>(2) )
{
}


template <concepts::Box PrimitiveType>
inline Size
CornerSampler<PrimitiveType>::size() const
{
    return _p_indexConverter->numberOfPoints();
}


template <concepts::Box PrimitiveType>
typename PrimitiveType::point_type
CornerSampler<PrimitiveType>::getSamplePoint( const PrimitiveType& r_primitive,
                                              const Size index ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto point = r_primitive.base();
    auto it_pointEnd = point.end();
    auto it_length = r_primitive.lengths().begin();

    auto it_index = this->_p_indexConverter->convert( index ).begin();

    for ( auto it_point=point.begin(); it_point!=it_pointEnd; ++it_point,++it_index,++it_length )
        *it_point += (*it_index) * (*it_length);

    return point;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg


#endif