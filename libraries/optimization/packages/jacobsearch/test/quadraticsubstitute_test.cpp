// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Linalg Includes ---
#include "linalg/packages/types/inc/vectortypes.hpp"

// --- Internal Includes ---
#include "optimization/packages/jacobsearch/inc/quadraticsubstitute.hpp"

// --- STL Includes ---
#include <iostream>

namespace cie::opt {


double paraboloidTestFunction(const DoubleArray<2>& position, const DoubleArray<2>& offset){
    return  5*(position[0]-offset[0]) * (position[0]-offset[0]) 
            + 2*(position[1]-offset[1]) * (position[1]-offset[1]);
}


CIE_TEST_CASE( "QuadraticSubstitute", "[jacobsearch]" )
{
    // TEST ON A SIMPLE PARABOLOID

    // Define paraboloid offset
    const DoubleArray<2>  offset  = { 2.0, 3.0 };
    // Define data sites
    const DoubleArray<2>  p1      = { 3.0, 6.0 };
    const DoubleArray<2>  p2      = { 4.0, 9.0 };
    const DoubleArray<2>  p3      = { 5.0, 12.0 };
    // Get values
    const double        v1      = paraboloidTestFunction( p1, offset );
    const double        v2      = paraboloidTestFunction( p2, offset );
    const double        v3      = paraboloidTestFunction( p3, offset );

    // Create substitute function
    std::vector<const DoubleArray<2>*> dataSites  = {&p1, &p2, &p3};
    QuadraticSubstitute<2> func( dataSites, DoubleVector({v1,v2,v3}) );

    // Approximate minimum
    std::pair<DoubleArray<2>, double> minimum;
    REQUIRE_NOTHROW( minimum = func.minimum() );

    // Check minimum position and function value there
    CHECK( minimum.first[0] == Approx(offset[0]) );
    CHECK( minimum.first[1] == Approx(offset[1]) );
    CHECK( paraboloidTestFunction(minimum.first, offset) == Approx(0.0) );
}

} // namespace cie::opt