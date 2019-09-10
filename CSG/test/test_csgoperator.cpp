#include "catch.hpp"
#include "csgoperator.hpp"
#include "box.hpp"
#include "sphere.hpp"

namespace csg{

TEST_CASE("CSG Operators"){

    // Define test point
    Point64 testPoint(1.5,0.5,0.5);

    // Define boxes
    Box* box1 = new Box( Point64({0.0,0.0,0.0}), Point64({1.0,1.0,1.0}) );
    Box* box2 = new Box( Point64({-1.0,-1.0,-1.0}), Point64({2.0,2.0,2.0}) );

    // Test boxes
    CHECK( box1->isInside(testPoint) == false );
    CHECK( box2->isInside(testPoint) == true  );
    
    // Create CSG operator objects
    CSGOperator* geom1    = new CSGUnion(box1,box2);
    CSGOperator* geom2    = new CSGSubtract(box1,box2);
    CSGOperator* geom3    = new CSGSubtract(box2,box1);
    CSGOperator* geom4    = new CSGIntersect(box1,box2);

    // Check CSG operators
    CHECK( geom1->isInside(testPoint) == true );
    CHECK( geom2->isInside(testPoint) == false );
    CHECK( geom3->isInside(testPoint) == true );
    CHECK( geom4->isInside(testPoint) == false );

}

}