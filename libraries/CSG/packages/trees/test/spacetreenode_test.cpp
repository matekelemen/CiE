// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/spacetreenode.hpp"
#include "CSG/packages/trees/inc/cell.hpp"
#include "CSG/packages/trees/inc/primitive_sampler.hpp"
#include "cmake_variables.hpp"

// --- STL includes ---
#include <deque>


namespace cie::csg {


TEST_CASE( "SpaceTreeNode", "[trees]" )
{
    CIE_TEST_CASE_INIT( "SpaceTreeNode" )

    // Basic parameters
    using CoordinateType                = Double;
    using ValueType                     = Bool;
    const Size Dimension                = 2;
    Size numberOfPointsPerDimension     = 3;

    // Target function (unit circle at the origin)
    auto unitCircle = []( const DoubleArray<Dimension>& point ) -> Bool
    {
        return (point[0]*point[0] + point[1]*point[1]) <= 1.0 ? true : false;
    };

    // Node with cube cell
    {
        CIE_TEST_CASE_INIT( "Cube base with midpoint splitting" )

        // Define specialised types
        using PrimitiveType = Cube<Dimension,CoordinateType>;
        using CellType      = CubeCell<PrimitiveType>;
        using NodeType      = SpaceTreeNode<CellType,ValueType>;

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
        CHECK_NOTHROW( root.evaluate(unitCircle) );
        REQUIRE( root.values().size() == 9 );

        CHECK( root.values()[0] == true );  // <-- { 0.0, 0.0 }
        CHECK( root.values()[1] == true );  // <-- { 0.5, 0.0 }
        CHECK( root.values()[2] == true );  // <-- { 1.0, 0.0 }
        CHECK( root.values()[3] == true );  // <-- { 0.0, 0.5 }
        CHECK( root.values()[4] == true );  // <-- { 0.5, 0.5 }
        CHECK( root.values()[5] == false ); // <-- { 1.0, 0.5 }
        CHECK( root.values()[6] == true );  // <-- { 0.0, 1.0 }
        CHECK( root.values()[7] == false ); // <-- { 0.5, 1.0 }
        CHECK( root.values()[8] == false ); // <-- { 1.0, 1.0 }

        CHECK( root.isBoundary() );

        // Divide twice
        CHECK_NOTHROW( root.divide(unitCircle, 2) );
    }
}


} // namespace cie::csg
