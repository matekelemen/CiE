#ifndef CIE_CSG_SPLIT_POLICY_HPP
#define CIE_CSG_SPLIT_POLICY_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/iterator_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/primitive_sampler.hpp"

// --- STL Includes ---
#include <functional>


namespace cie::csg {



// Abstract split policy
template <  class PointIterator,
            concepts::IteratorType ValueIterator>
class AbsSplitPolicy
{
public:
    typedef PointIterator                       point_iterator_type;
    typedef ValueIterator                       value_iterator_type;
    typedef typename PointIterator::value_type  point_type;
    typedef typename ValueIterator::value_type  value_type;

public:
    virtual point_type operator()(  point_iterator_type pointBegin,
                                    point_iterator_type pointEnd,
                                    value_iterator_type valueBegin ) const = 0;
};



// Midpoint splitter
template <  class PointIterator,
            concepts::IteratorType ValueIterator>
{
public:
    virtual typename MidpointSplitPolicy::point_type operator()( 
                typename MidpointSplitPolicy::point_iterator pointBegin,
                typename MidpointSplitPolicy::point_iterator pointEnd,
                typename MidpointSplitPolicy::value_iterator valueEnd ) const override;
};


}

#include "../impl/split_policy_impl.hpp"

#endif