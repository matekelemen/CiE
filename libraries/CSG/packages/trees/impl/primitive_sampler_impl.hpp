#ifndef CIE_CSG_PRIMITIVE_SAMPLER_IMPL_HPP
#define CIE_CSG_PRIMITIVE_SAMPLER_IMPL_HPP

// --- Internal Includes ---
#include "CSG/packages/trees/inc/indexconverter.hpp"

namespace cie::csg {

// ---------------------------------------------------------
// PRIMITIVE SAMPLER INTERFACE
// ---------------------------------------------------------

template <concepts::PrimitiveType PrimitiveType>
GridSampler<PrimitiveType>::GridSampler( Size numberOfPointsPerDimension )
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
    this->_size = intPow( this->_numberOfPointsPerDimension, Dimension );
    return *this;
}


// ---------------------------------------------------------
// SPECIALIZED PRIMITIVE SAMPLERS
// ---------------------------------------------------------

template <  Size Dimension,
            concepts::NumericType CoordinateType >
inline typename CubeSampler<Dimension,CoordinateType>::point_type
CubeSampler<Dimension,CoordinateType>::getSamplePoint(
        const typename CubeSampler<Dimension,CoordinateType>::primitive_type& primitive,
        Size index ) const
{
    typename CubeSampler<Dimension,CoordinateType>::point_type point;
    auto baseIt     = primitive.base().begin();

    if (this->_numberOfPointsPerDimension == 1)    // center point
        for (auto it=point.begin(); it!=point.end(); ++it,++baseIt)
            *it = *baseIt + primitive.length() / CoordinateType(2);
    else
    {
        auto indexIt    = SpaceTreeIndexConverter<Dimension,this->_numberOfPointsPerDimension>::convert(index).begin();

        for (auto it=point.begin(); it!=point.end(); ++it,++indexIt,++baseIt)
            *it = *baseIt + (*indexIt) * primitive.length() / (CoordinateType(this->resolution)-1.0);
    }

    return point;
}




template <  Size N, 
            Size M,
            concepts::NumericType CoordinateType >
inline typename BoxSampler<N,M,CoordinateType>::point_type
BoxSampler<N,M,CoordinateType>::operator()(
        const typename BoxSampler<N,M,CoordinateType>::primitive_type& primitive,
        Size index ) const
{
    typename BoxSampler<N,M,CoordinateType>::point_type point;
    auto baseIt     = primitive.base().begin();
    auto lengthIt   = primitive.lengths().begin();

    if (this->resolution == 1)    // center point
        for (auto it=point.begin(); it!=point.end(); ++it,++baseIt,++lengthIt)
            *it = *baseIt + (*lengthIt) / CoordinateType(2);
    else
    {
        auto indexIt    = SpaceTreeIndexConverter<this->dimension,this->resolution>::convert(index).begin();

        for (auto it=point.begin(); it!=point.end(); ++it,++indexIt,++baseIt,++lengthIt)
            *it = *baseIt + (*indexIt) * (*lengthIt) / (CoordinateType(this->resolution)-1.0);
    }

    return point;
}


} // namespace cie::csg

#endif