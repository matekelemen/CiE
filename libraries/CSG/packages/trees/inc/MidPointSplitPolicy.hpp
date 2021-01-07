#ifndef CIE_CSG_TREES_MID_POINT_SPLIT_POLICY_HPP
#define CIE_CSG_TREES_MID_POINT_SPLIT_POLICY_HPP

// --- Internal Includes ---
#include "CSG/packages/trees/inc/SplitPolicy.hpp"


namespace cie::csg {


/// Compute the arithmetic mean of the input points.
template <  concepts::IteratorType PointIterator,
            concepts::IteratorType ValueIterator>
class MidPointSplitPolicy : public SplitPolicy<PointIterator,ValueIterator>
{
public:
    virtual typename MidPointSplitPolicy<PointIterator,ValueIterator>::point_type operator()( 
                ValueIterator it_valueBegin,
                ValueIterator it_valueEnd,
                PointIterator it_pointBegin ) const override;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/MidPointSplitPolicy_impl.hpp"

#endif