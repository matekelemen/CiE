#ifndef CIE_CSG_SPLIT_POLICY_IMPL_HPP
#define CIE_CSG_SPLIT_POLICY_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>
#include <numeric>
#include <limits>


namespace cie::csg {


/* --- MidPointSplitPolicy --- */

template <  concepts::IteratorType PointIterator,
            concepts::IteratorType ValueIterator >
inline typename MidPointSplitPolicy<PointIterator,ValueIterator>::point_type
MidPointSplitPolicy<PointIterator,ValueIterator>:: operator()(
            ValueIterator it_valueBegin,
            ValueIterator it_valueEnd,
            PointIterator it_pointBegin ) const
{
    using PointType         = typename PointIterator::value_type;
    using CoordinateType    = typename PointType::value_type;

    PointType splitPoint;
    cie::utils::resize( splitPoint, it_pointBegin->size() );
    std::fill(  splitPoint.begin(),
                splitPoint.end(),
                0.0 );

    CoordinateType numberOfPoints = std::distance( it_valueBegin, it_valueEnd );

    for ( ; it_valueBegin!=it_valueEnd; it_valueBegin++,it_pointBegin++ )
    {
        auto it_tmp = it_pointBegin->begin();
        for (auto it_coordinate=splitPoint.begin(); it_coordinate!=splitPoint.end(); ++it_coordinate,++it_tmp)
            *it_coordinate += *it_tmp;
    }

    for (auto& r_coordinate : splitPoint)
        r_coordinate /= numberOfPoints;

    return splitPoint;
}


/* --- LinearSplitPolicy --- */

template <  concepts::IteratorType PointIterator,
            concepts::IteratorType ValueIterator >
inline typename LinearSplitPolicy<PointIterator,ValueIterator>::point_type
LinearSplitPolicy<PointIterator,ValueIterator>::operator()( ValueIterator it_valueBegin,
                                                            ValueIterator it_valueEnd,
                                                            PointIterator it_pointBegin ) const
{
    using PointType         = typename PointIterator::value_type;
    using CoordinateType    = typename PointType::value_type;

    // Init
    CoordinateType valueSum = std::accumulate(  it_valueBegin,
                                                 it_valueEnd,
                                                 0 );
    CoordinateType numberOfPoints = std::distance(  it_valueBegin,
                                                    it_valueEnd );

    // Initialize other components
    PointType componentSums, componentSquaredSums, mixedSums, min, max;
    std::fill(  componentSums.begin(),
                componentSums.end(),
                0 );
    std::fill(  componentSquaredSums.begin(),
                componentSquaredSums.end(),
                0 );
    std::fill(  mixedSums.begin(),
                mixedSums.end(),
                0 );
    std::fill(  min.begin(),
                min.end(),
                std::numeric_limits<CoordinateType>::max() );
    std::fill(  max.begin(),
                max.end(),
                std::numeric_limits<CoordinateType>::min() );

    typename PointType::iterator 
        it_componentSums, it_componentSquaredSums, it_mixedSums, it_min, it_max;

    // Loop through points (and values)
    for ( ; it_valueBegin!=it_valueEnd; ++it_valueBegin,++it_pointBegin )
    {
        it_componentSums        = componentSums.begin();
        it_componentSquaredSums = componentSquaredSums.begin();
        it_mixedSums            = mixedSums.begin();
        it_min                  = min.begin();
        it_max                  = max.begin();

        // Loop through components of the current point
        for ( const auto& component : *it_pointBegin )
        {
            *it_componentSums++         += component;
            *it_componentSquaredSums++  += component * component;
            *it_mixedSums++             += component * (*it_valueBegin);

            if (component < *it_min)
                *it_min = component;
            if (*it_max < component)
                *it_max = component;

            ++it_min;
            ++it_max;
        }
    }

    // Compute and restrict split point componentwise
    PointType splitPoint;
    it_componentSums        = componentSums.begin();
    it_componentSquaredSums = componentSquaredSums.begin();
    it_mixedSums            = mixedSums.begin();
    it_min = min.begin();
    it_max = max.begin();

    for ( auto& r_component : splitPoint )
    {
        r_component = (*it_mixedSums) - (*it_componentSums)*valueSum / numberOfPoints;  // <-- denominator
        if (r_component == 0)                                                           // <-- horizontal line -> get midpoint for this component
            r_component = *it_componentSums / numberOfPoints;
        else
        {
            r_component = 
            (
                (*it_componentSquaredSums) - (*it_componentSums)*(*it_componentSums) / numberOfPoints
            ) / r_component;
            r_component = ( (*it_componentSums) - valueSum * r_component ) / numberOfPoints;
        }

        if ( r_component < *it_min )
            r_component = *it_min;
        if ( *it_max < r_component )
            r_component = *it_max;

        ++it_componentSums;
        ++it_componentSquaredSums;
        ++it_mixedSums;
        ++it_min;
        ++it_max;
    }

    return splitPoint;
}



/* --- WeightedSplitPolicy --- */

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
        auto it_point       = it_pointBegin->begin();
        auto it_splitPoint  = splitPoint.begin();
        it_min              = min.begin();
        it_max              = max.begin();

        weight              = absMaxValue - (*it_valueBegin);
        weightSum           += weight;

        for ( ; it_point!=it_pointBegin->end(); ++it_point,++it_splitPoint,++it_min,++it_max )
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