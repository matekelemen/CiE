// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "optimization/packages/gradientdescent/inc/gradientestimate.hpp"

// --- STL Includes ---
#include <iostream>

namespace cie::opt {


double parabola(const RNRElement<2>& point)
{
    double out = 0.0;
    for (size_t i=0; i<2; ++i)
    {
        out += (point.getData()[i]-1.0) * (point.getData()[i]-1.0);
    }
    return out;
}


CIE_TEST_CASE( "Gradient estimation", "[gradientdescent]" )
{
    RNRObjectivePtr<2> objective = parabola;
    DoubleArray<2> gradient;

    CIE_TEST_REQUIRE_NOTHROW( gradient = gradientEstimate( RNRElement<2>({0.0,0.0}), objective, 1e-11 ) );
    CIE_TEST_CHECK( gradient[0] == Approx(gradient[1]) );
}

} // namespace cie::opt