#include "catch.hpp"
#include "../../inc/csgtypes.hpp"

namespace csg {


TEST_CASE("Point"){
    // Base constructor
    Point64 p1;
    for (uint i=0; i<p1.size(); ++i){
        CHECK( p1[i] == 0.0 );
    }
    // Constructor X
    Point64 p2(1.0);
    CHECK( p2[0] == 1.0 );
    for (uint i=1; i<p1.size(); ++i){
        CHECK( p2[i] == 0.0 );
    }
    // Constructor XY
    Point64 p3(1.0, 2.0);
    CHECK( p3[0] == 1.0 );
    CHECK( p3[1] == 2.0 );
    for (uint i=2; i<p1.size(); ++i){
        CHECK( p3[i] == 0.0 );
    }
    // Constructor XY
    Point64 p4(1.0, 2.0, 3.0);
    CHECK( p4[0] == 1.0 );
    CHECK( p4[1] == 2.0 );
    CHECK( p4[2] == 3.0 );
}


TEST_CASE("sgn"){
    float   f = -1.1;
    double  d = 0.0;
    int     i = 2;

    CHECK( sgn(f) == false );
    CHECK( sgn(d) == true );
    CHECK( sgn(i) == true );
}

}