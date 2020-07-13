#ifndef CIE_CSG_SPLIT_POLICY_IMPL_HPP
#define CIE_CSG_SPLIT_POLICY_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- STL Includes ---
#include <algorithm>


namespace cie::csg {


template <  class SamplerType,
            concepts::STLContainer ValueContainerType >
typename MidpointSplitPolicy<SamplerType,ValueContainerType>::point_type
MidpointSplitPolicy<SamplerType,ValueContainerType>:: operator()(
    const typename MidpointSplitPolicy<SamplerType,ValueContainerType>::primitive_type& primitive,
    const typename MidpointSplitPolicy<SamplerType,ValueContainerType>::value_container_type& values ) const
{
    typename MidpointSplitPolicy<SamplerType,ValueContainerType>::point_type point;
    utils::setContainerSize( point, SamplerType::dimension );
    std::fill( point.begin(), point.end(), typename SamplerType::coordinate_type(0) );

    auto numberOfSamples = typename SamplerType::coordinate_type(SamplerType::size());

    for (Size i=0; i<numberOfSamples; ++i)
    {
        auto tempPoint  = _sampler(primitive,i);
        auto pointIt    = point.begin();
        for (auto tempIt=tempPoint.begin(); tempIt!=tempPoint.end(); ++tempIt,++pointIt)
            *pointIt += *tempIt;
    }

    for (auto it=tempPoint.begin(); it!=tempPoint.end(); ++itimerspec)
            *it /= numberOfSamples;

    return point;
}


} // namespace cie::csg

#endif