// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "optimization/packages/objective/inc/objectivefunction.hpp"

// --- STL Includes ---
#include <iostream>

namespace cie::opt {

// Parabola offset to [1,1,1,...,1]^T
template <size_t N>
double parabolicObjective(const RNRElement<N>& point)
{
    double out = 0.0;
    for (size_t i=0; i<N; ++i)
    {
        out += (point.getData()[i]-1.0)*(point.getData()[i]-1.0);
    }
    return out;
}


template <size_t N>
DoubleArray<2> parabolicMultiObjective(const RNRMElement<N,2>& point)
{
	DoubleArray<2> out = {0.0,0.0};
     for (size_t i=0; i<N; ++i)
    {
        out[0] += (point.getData()[i]-1.0)*(point.getData()[i]-1.0);
        out[1] = -out[0];
    }
    return out;
}


CIE_TEST_CASE( "Objective", "[objective]" )
{
    RNRObjectivePtr<2> objective = parabolicObjective<2>;
    CIE_TEST_CHECK( objective( RNRElement<2>({1.0,1.0}) ) == Approx(0.0) );
    CIE_TEST_CHECK( objective( RNRElement<2>({0.0,0.0}) ) == Approx(2.0) );

    RNRMObjectivePtr<3,2> multiObjective = &parabolicMultiObjective<3>;
    CIE_TEST_CHECK( multiObjective( RNRMElement<3,2>({1.0,1.0,1.0}) )[0] == Approx(0.0) );
    CIE_TEST_CHECK( multiObjective( RNRMElement<3,2>({1.0,1.0,1.0}) )[1] == Approx(0.0) );
    CIE_TEST_CHECK( multiObjective( RNRMElement<3,2>({0.0,0.0,0.0}) )[0] == Approx(3.0) );
    CIE_TEST_CHECK( multiObjective( RNRMElement<3,2>({0.0,0.0,0.0}) )[1] == Approx(-3.0) );
}


} // namespace cie::opt