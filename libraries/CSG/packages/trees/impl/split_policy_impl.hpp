#ifndef CIE_CSG_SPLIT_POLICY_IMPL_HPP
#define CIE_CSG_SPLIT_POLICY_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- STL Includes ---
#include <algorithm>


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
    typename MidPointSplitPolicy<PointIterator,ValueIterator>::point_type splitPoint;
    cie::utils::resize( splitPoint, it_pointBegin->size() );
    std::fill(  splitPoint.begin(),
                splitPoint.end(),
                0.0 );

    Size numberOfPoints = std::distance( it_valueBegin, it_valueEnd );

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


} // namespace cie::csg

#endif