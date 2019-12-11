#include "catch.hpp"
#include "../inc/ntreenode.hpp"

namespace cie {
namespace csg {


bool unitCircle(const DoubleArray<2>& point)
{
    if ( point[0]*point[0] + point[1]*point[1] > 1.0)
        return false;
    else return true;
}


TEST_CASE("NTreeNode constructors")
{
    NTreeNode<2,3> root({0.0,0.0}, 2.0);
}


TEST_CASE("NTreeNode set/get")
{
    NTreeNode<2,3> root({1.0,2.0},3.0);
    REQUIRE( root.center().size() == 2 );
    CHECK( root.center()[0] == Approx(1.0) );
    CHECK( root.center()[1] == Approx(2.0) );
}


TEST_CASE("NTreeNode divide")
{
    NTreeNode<2, 3> root({ 1.0,1.0 }, 2.0);
    GeometryFunction<2> circle = unitCircle;

    root.divide(circle, 2);

    REQUIRE( root.children().size() == 4 );
    root.child(0).children().empty();
    //CHECK(root.child(0).children().size() == 4);
}


}
}