#ifndef CIE_CSG_PRIMITIVE_SAMPLER_IMPL_HPP
#define CIE_CSG_PRIMITIVE_SAMPLER_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- Internal Includes ---
#include "../inc/indexconverter.hpp"

namespace cie::csg {


// ---------------------------------------------------------
// SPECIALIZED PRIMITIVE SAMPLERS
// ---------------------------------------------------------

template <  Size N, 
            Size M,
            concepts::NumericType CoordinateType >
inline typename CubeSampler<N,M,CoordinateType>::point_type
CubeSampler<N,M,CoordinateType>::getSamplePoint(
        const typename CubeSampler<N,M,CoordinateType>::primitive_type& primitive,
        Size index ) const
{
    typename CubeSampler<N,M,CoordinateType>::point_type point;
    auto baseIt     = primitive.base().begin();

    if (this->resolution == 1)    // center point
        for (auto it=point.begin(); it!=point.end(); ++it,++baseIt)
            *it = *baseIt + primitive.length() / CoordinateType(2);
    else
    {
        auto indexIt    = SpaceTreeIndexConverter<this->dimension,this->resolution>::convert(index).begin();

        for (auto it=point.begin(); it!=point.end(); ++it,++indexIt,++baseIt)
            *it = *baseIt + (*indexIt) * primitive.length() / (CoordinateType(this->resolution)-1.0);
    }

    return point;
}



template <  Size N, 
            Size M,
            concepts::NumericType CoordinateType >
inline typename BoxSampler<N,M,CoordinateType>::point_type
BoxSampler<N,M,CoordinateType>::getSamplePoint(
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