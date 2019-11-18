#include "catch.hpp"
#include "quadraticsubstitute.hpp"

#include <iostream>

namespace opt {


double paraboloidTestFunction(const DoubleVector& position, const DoubleVector& offset){
    return  (position[0]-offset[0]) * (position[0]-offset[0]) 
            + (position[1]-offset[1]) * (position[1]-offset[1]);
}


TEST_CASE("QuadraticSubstitute"){
    // TEST ON A SIMPLE PARABOLOID

    // Define paraboloid offset
    const DoubleVector  offset  = { 2.0, 3.0 };
    // Define data sites
    const DoubleVector  p1      = { 3.0, 6.0 };
    const DoubleVector  p2      = { 4.0, 9.0 };
    const DoubleVector  p3      = { 5.0, 12.0 };
    // Get values
    const double        v1      = paraboloidTestFunction( p1, offset );
    const double        v2      = paraboloidTestFunction( p2, offset );
    const double        v3      = paraboloidTestFunction( p3, offset );

    // Create substitute function
    std::vector<const DoubleVector*> dataSites  = {&p1, &p2, &p3};
    QuadraticSubstitute func( dataSites, DoubleVector({v1,v2,v3}) );

    // Approximate minimum
    std::pair<DoubleVector, double> minimum;
    REQUIRE_NOTHROW( minimum = func.minimum() );

    // Check minimum position and function value there
    CHECK( minimum.first[0] == Approx(offset[0]) );
    CHECK( minimum.first[1] == Approx(offset[1]) );
    CHECK( paraboloidTestFunction(minimum.first, offset) == Approx(0.0) );
}

}