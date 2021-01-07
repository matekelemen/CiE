#ifndef CIE_CSG_TREES_WEIGHTED_SPLIT_POLICY_IMPL_HPP
#define CIE_CSG_TREES_WEIGHTED_SPLIT_POLICY_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>
#include <limits>


namespace cie::csg {


template <  concepts::IteratorType PointIterator,
            concepts::IteratorType ValueIterator >
inline typename WeightedSplitPolicy<PointIterator,ValueIterator>::point_type
WeightedSplitPolicy<PointIterator,ValueIterator>::operator()(   ValueIterator it_valueBegin,
                                                                ValueIterator it_valueEnd,
                                                                PointIterator it_pointBegin ) const
{
    using PointType             = typename PointIterator::value_type;
    using CoordinateType        = typename PointType::value_type;

    // Init
    CoordinateType absMaxValue  = 0.0;
    CoordinateType weightSum    = 0.0;
    CoordinateType weight;
    PointType splitPoint, min, max;
    typename PointType::iterator it_min, it_max;
    std::fill(  splitPoint.begin(),
                splitPoint.end(),
                0.0 );
    std::fill(  min.begin(),
                min.end(),
                std::numeric_limits<CoordinateType>::max() );
    std::fill(  max.begin(),
                max.end(),
                std::numeric_limits<CoordinateType>::min() );

    // First pass: find max abs value
    for ( ValueIterator it=it_valueBegin; it!=it_valueEnd; ++it )
        if ( absMaxValue < std::abs(*it) )
            absMaxValue = std::abs(*it);

    // Second pass: weight points and accumulate weights
    for ( ; it_valueBegin!=it_valueEnd; ++it_valueBegin,++it_pointBegin )
    {
        auto it_point               = it_pointBegin->begin();
        auto it_splitPoint          = splitPoint.begin();
        it_min                      = min.begin();
        it_max                      = max.begin();

        weight                      = absMaxValue - (*it_valueBegin);
        weightSum                   += weight;

        auto it_pointComponentEnd   = it_pointBegin->end();

        for ( ; it_point!=it_pointComponentEnd; ++it_point,++it_splitPoint,++it_min,++it_max )
        {
            *it_splitPoint += weight * (*it_point);

            if ( *it_point < *it_min )
                *it_min = *it_point;
            if ( *it_max < *it_point )
                *it_max = *it_point;
        }
    }

    // TODO: handle case where every value is 0
    if ( weightSum == 0 )
        weightSum = 1;

    // Average and bound
    it_min = min.begin();
    it_max = max.begin();

    for ( auto& r_component : splitPoint )
    {
        r_component /= weightSum;

        if ( r_component < *it_min )
            r_component = *it_min;
        else if ( *it_max < r_component )
            r_component = *it_max;

        ++it_min;
        ++it_max;
    }

    return splitPoint;
}


} // namespace cie::csg

#endif