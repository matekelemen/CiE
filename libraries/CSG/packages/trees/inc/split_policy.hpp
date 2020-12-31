#ifndef CIE_CSG_SPLIT_POLICY_HPP
#define CIE_CSG_SPLIT_POLICY_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/iterator_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/primitive_sampler.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::csg {


/* --- SplitPolicy --- */

/**
 * Interface for providing points around which
 * to split a cell. Generating the split point
 * is based on a set of point-value pairs and
 * must be implemented in derived classes.
*/
template <  concepts::IteratorType PointIterator,
            concepts::IteratorType ValueIterator>
class SplitPolicy
{
public:
    typedef PointIterator                       point_iterator_type;
    typedef ValueIterator                       value_iterator_type;
    typedef typename PointIterator::value_type  point_type;
    typedef typename ValueIterator::value_type  value_type;

public:
    virtual point_type operator()(  ValueIterator it_valueBegin,
                                    ValueIterator it_valueEnd,
                                    PointIterator it_pointBegin ) const = 0;
};



template <  concepts::IteratorType PointIterator,
            concepts::IteratorType ValueIterator >
using SplitPolicyPtr = std::shared_ptr<SplitPolicy<PointIterator,ValueIterator>>;



/* --- MidPointSplitPolicy --- */

/**
 * Compute the arithmetic mean of the input points.
*/
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

#include "CSG/packages/trees/impl/split_policy_impl.hpp"

#endif