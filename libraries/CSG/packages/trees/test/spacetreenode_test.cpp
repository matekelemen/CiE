// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/spacetreenode.hpp"
#include "CSG/packages/trees/inc/cell.hpp"
#include "CSG/packages/trees/inc/primitive_sampler.hpp"
#include "CSG/packages/trees/inc/write.hpp"
#include "cmake_variables.hpp"

// --- STL includes ---
#include <deque>
#include <concepts>


namespace cie::csg {


template <class PointType, class ValueType>
requires (!std::is_same_v<ValueType,Bool>)
ValueType unitCircle( const PointType& r_point )
{
    typename PointType::value_type value = 0.0;
    for ( const auto& r_component : r_point )
        value += r_component*r_component;
    
    return ValueType(1.0 - value);
}


template <class PointType, class ValueType>
requires std::is_same_v<ValueType,Bool>
ValueType unitCircle( const PointType& r_point )
{
    return unitCircle<PointType,Float>(r_point) >= 0;
}


TEST_CASE( "SpaceTreeNode", "[trees]" )
{
    CIE_TEST_CASE_INIT( "SpaceTreeNode" )

    // Basic parameters
    const Size Dimension                = 2;
    using CoordinateType                = Double;
    using PointType                     = std::array<CoordinateType,Dimension>;
    using BoolValue                     = Bool;
    using NumericValue                  = Float;
    Size numberOfPointsPerDimension     = 3;
    Size depth                          = 5;

    // Node with cube cell
    {
        CIE_TEST_CASE_INIT( "Cube base with midpoint splitting" )

        // Define specialised types
        using PrimitiveType = Cube<Dimension,CoordinateType>;
        using CellType      = CubeCell<PrimitiveType>;
        using NodeType      = SpaceTreeNode<CellType,BoolValue>;

        // Define primitive constructor parameters
        typename PrimitiveType::point_type base = { 0.0, 0.0 };
        CoordinateType length                   = 1.0;

        // Construct components
        auto p_sampler = typename NodeType::sampler_ptr(
            new CubeSampler<Dimension,Double>(numberOfPointsPerDimension)
        );

        auto p_splitPolicy = typename NodeType::split_policy_ptr(
            new MidPointSplitPolicy<typename NodeType::sample_point_iterator,
                                    typename NodeType::value_iterator>()
        );

        // Construct a node
        REQUIRE_NOTHROW(NodeType(   p_sampler,
                                    p_splitPolicy,
                                    0,
                                    base,
                                    length ));
        NodeType root(  p_sampler,
                        p_splitPolicy,
                        0,
                        base,
                        length );

        CHECK_THROWS( root.isBoundary() );

        // Evaluate unit circle
        CHECK_NOTHROW( root.evaluate(unitCircle<PointType,BoolValue>) );
        REQUIRE( root.values().size() == 9 );
        CHECK( root.isBoundary() );

        CHECK( root.values()[0] == true );  // <-- { 0.0, 0.0 }
        CHECK( root.values()[1] == true );  // <-- { 0.5, 0.0 }
        CHECK( root.values()[2] == true );  // <-- { 1.0, 0.0 }
        CHECK( root.values()[3] == true );  // <-- { 0.0, 0.5 }
        CHECK( root.values()[4] == true );  // <-- { 0.5, 0.5 }
        CHECK( root.values()[5] == false ); // <-- { 1.0, 0.5 }
        CHECK( root.values()[6] == true );  // <-- { 0.0, 1.0 }
        CHECK( root.values()[7] == false ); // <-- { 0.5, 1.0 }
        CHECK( root.values()[8] == false ); // <-- { 1.0, 1.0 }

        // Divide
        CHECK_NOTHROW( root.divide(unitCircle<PointType,BoolValue>, depth) );
        
        // Write output
        CHECK_NOTHROW( writeToVTK( root, TEST_OUTPUT_PATH + "/SpaceTreeNode_cube_midpoint.vtu" ) );
    }

    // Node with box cell
    {
        CIE_TEST_CASE_INIT( "Box base" )

        // Define specialised types
        using PrimitiveType = Box<Dimension,CoordinateType>;
        using CellType      = BoxCell<PrimitiveType>;

        { // Midpoint splitting
            CIE_TEST_CASE_INIT( "Midpoint splitter" )

            using NodeType      = SpaceTreeNode<CellType,BoolValue>;

            // Define primitive constructor parameters
            typename PrimitiveType::point_type base     = { 0.0, 0.0 };
            typename PrimitiveType::point_type lengths  = { 1.0, 1.0 };

            // Construct components
            auto p_sampler = typename NodeType::sampler_ptr(
                new BoxSampler<Dimension,Double>(numberOfPointsPerDimension)
            );

            auto p_splitPolicy = typename NodeType::split_policy_ptr(
                new MidPointSplitPolicy<typename NodeType::sample_point_iterator,
                                        typename NodeType::value_iterator>()
            );

            // Construct a node
            REQUIRE_NOTHROW(NodeType(   p_sampler,
                                        p_splitPolicy,
                                        0,
                                        base,
                                        lengths ));
            NodeType root(  p_sampler,
                            p_splitPolicy,
                            0,
                            base,
                            lengths );

            CHECK_THROWS( root.isBoundary() );

            // Divide
            CHECK_NOTHROW( root.divide(unitCircle<PointType,BoolValue>, depth) );
            
            // Write output
            CHECK_NOTHROW( writeToVTK( root, TEST_OUTPUT_PATH + "/SpaceTreeNode_box_midpoint.vtu" ) );
        } // Midpoint splitting

        { // Linear splitting
            CIE_TEST_CASE_INIT( "Linear splitter" )

            using NodeType      = SpaceTreeNode<CellType,NumericValue>;

            // Define primitive constructor parameters
            typename PrimitiveType::point_type base     = { 0.0, 0.0 };
            typename PrimitiveType::point_type lengths  = { 1.0, 1.0 };

            // Construct components
            auto p_sampler = typename NodeType::sampler_ptr(
                new BoxSampler<Dimension,Double>(numberOfPointsPerDimension)
            );

            auto p_splitPolicy = typename NodeType::split_policy_ptr(
                new LinearSplitPolicy< typename NodeType::sample_point_iterator,
                                       typename NodeType::value_iterator>()
            );

            // Construct a node
            REQUIRE_NOTHROW(NodeType(   p_sampler,
                                        p_splitPolicy,
                                        0,
                                        base,
                                        lengths ));
            NodeType root(  p_sampler,
                            p_splitPolicy,
                            0,
                            base,
                            lengths );

            CHECK_THROWS( root.isBoundary() );

            // Divide
            CHECK_NOTHROW( root.divide(unitCircle<PointType,NumericValue>, depth) );
            
            // Write output
            CHECK_NOTHROW( writeToVTK( root, TEST_OUTPUT_PATH + "/SpaceTreeNode_box_linear.vtu" ) );
        } // Linear splitting

        { // Weighted splitting
            CIE_TEST_CASE_INIT( "Weighted splitter" )

            using NodeType      = SpaceTreeNode<CellType,NumericValue>;

            // Define primitive constructor parameters
            typename PrimitiveType::point_type base     = { 0.0, 0.0 };
            typename PrimitiveType::point_type lengths  = { 1.0, 1.0 };

            // Construct components
            auto p_sampler = typename NodeType::sampler_ptr(
                new BoxSampler<Dimension,Double>(numberOfPointsPerDimension)
            );

            auto p_splitPolicy = typename NodeType::split_policy_ptr(
                new WeightedSplitPolicy< typename NodeType::sample_point_iterator,
                                         typename NodeType::value_iterator>()
            );

            // Construct a node
            REQUIRE_NOTHROW(NodeType(   p_sampler,
                                        p_splitPolicy,
                                        0,
                                        base,
                                        lengths ));
            NodeType root(  p_sampler,
                            p_splitPolicy,
                            0,
                            base,
                            lengths );

            CHECK_THROWS( root.isBoundary() );

            // Divide
            CHECK_NOTHROW( root.divide(unitCircle<PointType,NumericValue>, depth) );
            
            // Write output
            CHECK_NOTHROW( writeToVTK( root, TEST_OUTPUT_PATH + "/SpaceTreeNode_box_weighted.vtu" ) );
        } // Weighted splitting
        
    } // Node with box cell
}


} // namespace cie::csg
