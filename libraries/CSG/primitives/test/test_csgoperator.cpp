#include "catch.hpp"
#include "../inc/csgoperator.hpp"
#include "../inc/box.hpp"
#include "../inc/sphere.hpp"


namespace cie {
namespace csg{

TEST_CASE("CSG Operators"){

    // Define test point
    double xMin = -1.0;
    double xMax = 1.5;
    double dx   = 0.5;
    uint n      = (xMax-xMin)/dx + 1;

    // Define boxes
    Box* box1 = new Box( Point64({0.0,0.0,0.0}), Point64({1.0,1.0,1.0}) );
    //Box* box2 = new Box( Point64({-1.0,-1.0,-1.0}), Point64({2.0,2.0,2.0}) );

    // Test
    bool result;
    uint N;
    std::vector<Point64> points(n*n*n);
    for (uint i=0;i<n;++i){
        for (uint j=0;j<n;++j){
            for (uint k=0;k<n;++k){

                N = i*n*n+j*n+k;

                points[N] = Point64(
                    i*dx,
                    j*dx,
                    k*dx
                );

                result = true;
                for (uint I=0;I<3;++I){
                    if (points[N][I]<box1->getPoint(0)[I] || points[N][I]>box1->getPoint(1)[I]){
                        result = false;
                        break;
                    }
                }
                CHECK( box1->isInside(points[N]) == result );
            }
        }
    }

    /*
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
    */

}

}
}