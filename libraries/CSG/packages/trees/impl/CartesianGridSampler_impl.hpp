#ifndef CIE_CSG_TREES_CARTESIAN_GRID_SAMPLER_IMPL_HPP
#define CIE_CSG_TREES_CARTESIAN_GRID_SAMPLER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::csg {


template <concepts::Primitive PrimitiveType>
AbsCartesianGridSampler<PrimitiveType>::AbsCartesianGridSampler( Size numberOfPointsPerDimension ) :
    _p_indexConverter( new CartesianIndexConverter<PrimitiveType::dimension>(numberOfPointsPerDimension) ),
    _numberOfPointsPerDimension( numberOfPointsPerDimension )
{
    this->setNumberOfPointsPerDimension( numberOfPointsPerDimension );
}


template <concepts::Primitive PrimitiveType>
inline Size
AbsCartesianGridSampler<PrimitiveType>::size() const
{
    return this->_size;
}


template <concepts::Primitive PrimitiveType>
inline Size
AbsCartesianGridSampler<PrimitiveType>::numberOfPointsPerDimension() const
{
    return this->_numberOfPointsPerDimension;
}


template <concepts::Primitive PrimitiveType>
void
AbsCartesianGridSampler<PrimitiveType>::setNumberOfPointsPerDimension( Size numberOfPointsPerDimension )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_numberOfPointsPerDimension = numberOfPointsPerDimension;
    this->_size = intPow( this->_numberOfPointsPerDimension, PrimitiveType::dimension );
    _p_indexConverter.reset( new CartesianIndexConverter<PrimitiveType::dimension>(numberOfPointsPerDimension) );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::Primitive PrimitiveType>
const CartesianIndexConverter<PrimitiveType::dimension>&
AbsCartesianGridSampler<PrimitiveType>::indexConverter() const
{
    return *this->_p_indexConverter;
}


// ---------------------------------------------------------
// SPECIALIZED PRIMITIVE SAMPLERS
// ---------------------------------------------------------

/* --- Cube sampler --- */

template <concepts::Cube PrimitiveType>
CartesianGridSampler<PrimitiveType>::CartesianGridSampler( Size numberOfPointsPerDimension ) :
    AbsCartesianGridSampler<PrimitiveType>( numberOfPointsPerDimension )
{
}


template <concepts::Cube PrimitiveType>
inline typename CartesianGridSampler<PrimitiveType>::point_type
CartesianGridSampler<PrimitiveType>::getSamplePoint( const PrimitiveType& r_primitive,
                                                     Size index ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    using CoordinateType = typename CartesianGridSampler<PrimitiveType>::coordinate_type;

    typename CartesianGridSampler<PrimitiveType>::point_type point;
    auto it_base     = r_primitive.base().begin();
    auto it_pointEnd = point.end();

    if (this->numberOfPointsPerDimension() == 1)    // center point
        for (auto it=point.begin(); it!=it_pointEnd; ++it,++it_base)
            *it = *it_base + r_primitive.length() / CoordinateType(2);
    else
    {
        auto it_index   = this->indexConverter().convert( index ).begin();

        for (auto it=point.begin(); it!=it_pointEnd; ++it,++it_index,++it_base)
            *it = *it_base + (*it_index) * r_primitive.length() / (CoordinateType(this->numberOfPointsPerDimension())-1.0);
    }

    return point;

    CIE_END_EXCEPTION_TRACING
}



/* --- Box sampler --- */

template <concepts::Box PrimitiveType>
CartesianGridSampler<PrimitiveType>::CartesianGridSampler( Size numberOfPointsPerDimension ) :
    AbsCartesianGridSampler<PrimitiveType>( numberOfPointsPerDimension )
{
}

template <concepts::Box PrimitiveType>
inline typename CartesianGridSampler<PrimitiveType>::point_type
CartesianGridSampler<PrimitiveType>::getSamplePoint( const PrimitiveType& r_primitive,
                                                     Size index ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    using CoordinateType = typename CartesianGridSampler<PrimitiveType>::coordinate_type;

    typename CartesianGridSampler<PrimitiveType>::point_type point;
    auto it_base        = r_primitive.base().begin();
    auto it_length      = r_primitive.lengths().begin();
    auto it_pointEnd    = point.end();

    if (this->numberOfPointsPerDimension() == 1)    // center point
        for (auto it=point.begin(); it!=it_pointEnd; ++it,++it_base,++it_length)
            *it = *it_base + (*it_length) / CoordinateType(2);
    else
    {
        auto it_index    = this->indexConverter().convert(index).begin();

        for (auto it=point.begin(); it!=it_pointEnd; ++it,++it_index,++it_base,++it_length)
            *it = (*it_base) + (*it_index) * (*it_length) / (CoordinateType(this->numberOfPointsPerDimension())-1.0);
    }

    return point;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg


#endif