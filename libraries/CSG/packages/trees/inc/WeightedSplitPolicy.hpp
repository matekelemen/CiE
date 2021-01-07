#ifndef CIE_CSG_TREES_WEIGHTED_SPLIT_POLICY_HPP
#define CIE_CSG_TREES_WEIGHTED_SPLIT_POLICY_HPP

// --- Internal Includes ---
#include "CSG/packages/trees/inc/SplitPolicy.hpp"


namespace cie::csg {


/**
 * Compute a weighted average of the sample point set.
 * ( modified weights -> abs and reverse values )
*/
template <  concepts::IteratorType PointIterator,
            concepts::IteratorType ValueIterator >
class WeightedSplitPolicy : public SplitPolicy<PointIterator,ValueIterator>
{
public:
    virtual typename WeightedSplitPolicy<PointIterator,ValueIterator>::point_type operator()(
        ValueIterator it_valueBegin,
        ValueIterator it_valueEnd,
        PointIterator it_pointBegin ) const override;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/WeightedSplitPolicy_impl.hpp"

#endif