#ifndef CIE_CSG_SPLIT_POLICY_HPP
#define CIE_CSG_SPLIT_POLICY_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- Internal Includes ---
#include "./primitive_sampler.hpp"


namespace cie::csg {


// Forward declare SpaceTreeNode
template <  class CellType,
            class SamplerType,
            class SplitPolicy,
            concepts::STLContainer ValueContainerType >
class SpaceTreeNode;



// Abstract split policy
template <  class SamplerType,
            concepts::STLContainer ValueContainerType >
class AbsSplitPolicy
{
public:
    typedef SamplerType                             sampler_type;
    typedef typename sampler_type::primitive_type   primitive_type;
    typedef typename sampler_type::point_type       point_type;
    typedef ValueContainerType                      value_container_type;

public:
    virtual point_type operator()(  const primitive_type& primitive,
                                    const value_container_type& values ) const = 0;

protected:
    static const SamplerType _sampler;
};



// Midpoint splitter
template <  class SamplerType,
            concepts::STLContainer ValueContainerType >
class MidpointSplitPolicy : public AbsSplitPolicy<SamplerType,ValueContainerType>
{
public:
    virtual typename MidpointSplitPolicy::point_type operator()( 
        const typename MidpointSplitPolicy::primitive_type& primitive,
        const typename MidpointSplitPolicy::value_container_type& values ) const override;

protected:
    typename SamplerType::sampler_base_type
};


}

#include "../impl/split_policy_impl.hpp"

#endif