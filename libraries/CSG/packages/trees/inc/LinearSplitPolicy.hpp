#ifndef CIE_CSG_TREES_LINEAR_SPLIT_POLICY_HPP
#define CIE_CSG_TREES_LINEAR_SPLIT_POLICY_HPP

// --- Internal Includes ---
#include "CSG/packages/trees/inc/SplitPolicy.hpp"


namespace cie::csg {


/**
 * Find the root of the least squares line of each component
 * and bound it by their limits. 
*/
template <  concepts::IteratorType PointIterator,
            concepts::IteratorType ValueIterator >
class LinearSplitPolicy : public SplitPolicy<PointIterator,ValueIterator>
{
public:
    virtual typename LinearSplitPolicy<PointIterator,ValueIterator>::point_type operator()(
        ValueIterator it_valueBegin,
        ValueIterator it_valueEnd,
        PointIterator it_pointBegin ) const override;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/LinearSplitPolicy_impl.hpp"

#endif