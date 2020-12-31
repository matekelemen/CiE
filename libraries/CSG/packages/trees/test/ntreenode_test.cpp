// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/spacetreenode.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <iostream>

namespace cie::csg {

/*
#pragma acc routine seq
double unitCircle(const DoubleArray<2>& point)
{
    return (point[0]*point[0]+point[1]*point[1]) - 1.0;
}


#pragma acc routine seq
double unitSphere(const DoubleArray<3>& point)
{
    return (point[0]*point[0]+point[1]*point[1]+point[2]*point[2]) - 1.0;
}


CIE_TEST_CASE("SpaceTreeNode constructors")
{
    SpaceTreeNode<2,3> root({0.0,0.0}, 2.0);
}


CIE_TEST_CASE("SpaceTreeNode set/get")
{
    SpaceTreeNode<2,3> root({1.0,2.0},3.0);
    CIE_TEST_REQUIRE( root.center().size() == 2 );
    CIE_TEST_CHECK( root.center()[0] == Approx(1.0) );
    CIE_TEST_CHECK( root.center()[1] == Approx(2.0) );
}


CIE_TEST_CASE("SpaceTreeNode divide 2D")
{
    // 2---+---+
    // |   |   |
    // 1---c---+
    // |   |   |
    // 0---1---2

    // Create geometry
    SpaceTreeNode<2, 3> root({ 1.0,1.0 }, 2.0);
    GeometryFunction<2> circle = unitCircle;

    // Initialize and divide tree
    root.evaluate(circle);
    root.divide(circle, 10);

    // Check root data
    CIE_TEST_REQUIRE( root.data().size() == 9 );

    CIE_TEST_CHECK( root.data()[0] == Approx(-1.0) );
    CIE_TEST_CHECK( root.data()[1] == Approx(0.0) );
    CIE_TEST_CHECK( root.data()[2] == Approx(3.0) );

    CIE_TEST_CHECK( root.data()[3] == Approx(0.0) );
    CIE_TEST_CHECK( root.data()[4] == Approx(1.0) );
    CIE_TEST_CHECK( root.data()[5] == Approx(4.0) );

    CIE_TEST_CHECK( root.data()[6] == Approx(3.0) );
    CIE_TEST_CHECK( root.data()[7] == Approx(4.0) );
    CIE_TEST_CHECK( root.data()[8] == Approx(7.0) );

    // Check division on first level
    CIE_TEST_REQUIRE( root.children()[0] != nullptr );
    CIE_TEST_REQUIRE( root.children().size() == 4 );

    // Check division on second level
    CIE_TEST_CHECK(root.child(0).children().size() == 4);

    // Check writing to file
    CIE_TEST_CHECK_NOTHROW(writeSpaceTree<2,3>(root, TEST_OUTPUT_PATH / "spacetree2D.csv"));
}


CIE_TEST_CASE("SpaceTreeNode divide 3D")
{
    {
        // Create geometry
        SpaceTreeNode<3, 3> root({ 1.0,1.0,1.0 }, 2.0);
        GeometryFunction<3> sphere = unitSphere;

        // Initialize and divide tree
        root.evaluate(sphere);
        root.divide(sphere, 7);
    }

    {
        // Create geometry
        SpaceTreeNode<3, 3> root({ 1.0,1.0,1.0 }, 2.0);
        GeometryFunction<3> sphere = unitSphere;

        // Initialize and divide tree
        root.evaluate(sphere);
        root.divideOffload(sphere, 7);

        // Check writing to file
        CIE_TEST_CHECK_NOTHROW(writeSpaceTree<3,3>(root, TEST_OUTPUT_PATH / "spacetree3D.csv"));
    }
}


CIE_TEST_CASE("Boundary nodes")
{
    // Create geometry
    SpaceTreeNode<2, 3> root({ 1.0,1.0 }, 2.0);
    GeometryFunction<2> circle = unitCircle;

    // Initialize and divide tree
    root.evaluate(circle);
    root.divide(circle, 4);

    auto nodes = boundaryNodes<2,3>(root);
}
*/

} // namespace cie::csg