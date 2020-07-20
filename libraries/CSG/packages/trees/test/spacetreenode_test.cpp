// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/spacetreenode.hpp"
#include "../inc/primitive_sampler.hpp"
#include "../inc/split_policy.hpp"
#include "../inc/cell.hpp"
#include "cmake_variables.hpp"

// --- STL includes ---
#include <deque>

/*
namespace cie::csg {


class TestNode : public SpaceTreeNode_
    <
        boolean::CubeCell<2>,
        CubeSampler<2,2>,
        MidpointSplitPolicy<CubeSampler<2,2>,std::deque<bool>>
    >
{
public:
    typedef SpaceTreeNode_
    <
        boolean::CubeCell<2>,
        CubeSampler<2,2>,
        MidpointSplitPolicy<CubeSampler<2,2>,std::deque<bool>>
    > node_base_type;

public:
    template <concepts::NumericContainer PointType>
    TestNode(   const PointType& base,
                typename boolean::CubeCell<2>::coordinate_type length ) 
        : node_base_type(base,length) 
    {}


};



TEST_CASE( "SpaceTreeNode_", "[trees]" )
{
    std::vector<Double> base = {0.0, 0.0};
    Double length = 1.0;
    TestNode root(base, length);
}


} // namespace cie::csg
*/