#ifndef CIE_CSG_PRIMITIVE_SAMPLER_IMPL_HPP
#define CIE_CSG_PRIMITIVE_SAMPLER_IMPL_HPP

// --- Internal Includes ---
#include "CSG/packages/trees/inc/indexconverter.hpp"

namespace cie::csg {

// ---------------------------------------------------------
// PRIMITIVE SAMPLER INTERFACE
// ---------------------------------------------------------

template <concepts::PrimitiveType PrimitiveType>
GridSampler<PrimitiveType>::GridSampler( Size numberOfPointsPerDimension ) :
    _p_indexConverter( new GridIndexConverter<PrimitiveType::dimension>(numberOfPointsPerDimension) ),
    _numberOfPointsPerDimension( numberOfPointsPerDimension )
{
    this->setNumberOfPointsPerDimension( numberOfPointsPerDimension );
}


template <concepts::PrimitiveType PrimitiveType>
inline Size
GridSampler<PrimitiveType>::size() const
{
    return this->_size;
}


template <concepts::PrimitiveType PrimitiveType>
inline Size
GridSampler<PrimitiveType>::numberOfPointsPerDimension() const
{
    return this->_numberOfPointsPerDimension;
}


template <concepts::PrimitiveType PrimitiveType>
GridSampler<PrimitiveType>&
GridSampler<PrimitiveType>::setNumberOfPointsPerDimension( Size numberOfPointsPerDimension )
{
    this->_numberOfPointsPerDimension = numberOfPointsPerDimension;
    this->_size = intPow( this->_numberOfPointsPerDimension, PrimitiveType::dimension );
    _p_indexConverter.reset( new GridIndexConverter<PrimitiveType::dimension>(numberOfPointsPerDimension) );
    return *this;
}


template <concepts::PrimitiveType PrimitiveType>
const GridIndexConverter<PrimitiveType::dimension>&
GridSampler<PrimitiveType>::indexConverter() const
{
    return *this->_p_indexConverter;
}


// ---------------------------------------------------------
// SPECIALIZED PRIMITIVE SAMPLERS
// ---------------------------------------------------------

/* --- CubeSampler --- */

template <  Size Dimension,
            concepts::NumericType CoordinateType >
CubeSampler<Dimension,CoordinateType>::CubeSampler( Size numberOfPointsPerDimension ) :
    GridSampler<Cube<Dimension,CoordinateType>>( numberOfPointsPerDimension )
{
}


template <  Size Dimension,
            concepts::NumericType CoordinateType >
inline typename CubeSampler<Dimension,CoordinateType>::point_type
CubeSampler<Dimension,CoordinateType>::getSamplePoint(
        const typename CubeSampler<Dimension,CoordinateType>::primitive_type& r_primitive,
        Size index ) const
{
    typename CubeSampler<Dimension,CoordinateType>::point_type point;
    auto it_base     = r_primitive.base().begin();

    if (this->numberOfPointsPerDimension() == 1)    // center point
        for (auto it=point.begin(); it!=point.end(); ++it,++it_base)
            *it = *it_base + r_primitive.length() / CoordinateType(2);
    else
    {
        auto it_index   = this->indexConverter().convert( index ).begin();

        for (auto it=point.begin(); it!=point.end(); ++it,++it_index,++it_base)
            *it = *it_base + (*it_index) * r_primitive.length() / (CoordinateType(this->numberOfPointsPerDimension())-1.0);
    }

    return point;
}



/* --- BoxSampler --- */

template <  Size Dimension,
            concepts::NumericType CoordinateType >
BoxSampler<Dimension,CoordinateType>::BoxSampler( Size numberOfPointsPerDimension ) :
    GridSampler<Box<Dimension,CoordinateType>>( numberOfPointsPerDimension )
{
}

template <  Size Dimension, 
            concepts::NumericType CoordinateType >
inline typename BoxSampler<Dimension,CoordinateType>::point_type
BoxSampler<Dimension,CoordinateType>::getSamplePoint(
        const typename BoxSampler<Dimension,CoordinateType>::primitive_type& r_primitive,
        Size index ) const
{
    typename BoxSampler<Dimension,CoordinateType>::point_type point;
    auto it_base    = r_primitive.base().begin();
    auto it_length  = r_primitive.lengths().begin();

    if (this->numberOfPointsPerDimension() == 1)    // center point
        for (auto it=point.begin(); it!=point.end(); ++it,++it_base,++it_length)
            *it = *it_base + (*it_length) / CoordinateType(2);
    else
    {
        auto it_index    = this->indexConverter().convert(index).begin();

        for (auto it=point.begin(); it!=point.end(); ++it,++it_index,++it_base,++it_length)
            *it = (*it_base) + (*it_index) * (*it_length) / (CoordinateType(this->numberOfPointsPerDimension())-1.0);
    }

    return point;
}


} // namespace cie::csg

#endif