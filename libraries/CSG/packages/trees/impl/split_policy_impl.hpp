#ifndef CIE_CSG_SPLIT_POLICY_IMPL_HPP
#define CIE_CSG_SPLIT_POLICY_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- STL Includes ---
#include <algorithm>


namespace cie::csg {



template <  class PointIterator,
            class ValueIterator >
inline typename MidpointSplitPolicy<PointIterator,ValueIterator>::point_type
MidpointSplitPolicy<PointIterator,ValueIterator>:: operator()(
            typename MidpointSplitPolicy<PointIterator,ValueIterator>::point_iterator pointBegin,
            typename MidpointSplitPolicy<PointIterator,ValueIterator>::point_iterator pointEnd,
            typename MidpointSplitPolicy<PointIterator,ValueIterator>::value_iterator valueBegin ) const
{
    typename MidpointSplitPolicy<PointIterator,ValueIterator>::point_type point;
    Size counter = 0;
    
    for ( ; pointBegin!=pointEnd; pointBegin++ )
    {
        auto tempIt = pointBegin->begin();
        for (auto coordinateIt=point.begin(); coordinateIt!=point.end(); ++coordinateIt,++tempIt)
        {
            ++counter;
            *coordinateIt += *tempIt;
        }
    }

    for (auto& coordinate : point)
        coordinate /= counter;

    return point;
}


} // namespace cie::csg

#endif