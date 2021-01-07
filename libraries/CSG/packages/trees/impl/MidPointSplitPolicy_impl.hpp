#ifndef CIE_CSG_TREES_MID_POINT_SPLIT_POLICY_IMPL_HPP
#define CIE_CSG_TREES_MID_POINT_SPLIT_POLICY_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::csg {


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
    auto it_splitPointBegin = splitPoint.begin();
    auto it_splitPointEnd   = splitPoint.end();

    cie::utils::resize( splitPoint, it_pointBegin->size() );
    std::fill(  it_splitPointBegin,
                it_splitPointEnd,
                0.0 );

    CoordinateType numberOfPoints = std::distance( it_valueBegin, it_valueEnd );

    for ( ; it_valueBegin!=it_valueEnd; it_valueBegin++,it_pointBegin++ )
    {
        auto it_tmp = it_pointBegin->begin();
        for (auto it_coordinate=it_splitPointBegin; it_coordinate!=it_splitPointEnd; ++it_coordinate,++it_tmp)
            *it_coordinate += *it_tmp;
    }

    for (auto& r_coordinate : splitPoint)
        r_coordinate /= numberOfPoints;

    return splitPoint;
}


} // namespace cie::csg

#endif