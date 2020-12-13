// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "optimization/packages/jacobsearch/inc/jacobsearch.hpp"
#include "optimization/packages/solver/inc/testfunctions.hpp"

// --- STL Includes ---
#include <iostream>

namespace cie::opt {


double jacobSearchTestFunction(const RNRElement<2>& point)
{
    double out = 0.0;
    for (size_t i=0; i<point.getData().size(); ++i)
    {
        out += (point.getData()[i]-1.0) * (point.getData()[i]-1.0);
    }
    return out;
}


CIE_TEST_CASE( "Jacob search - paraboloid_1", "[jacobsearch]" )
{
	// Objective definition
	RNRElement<2>		offset({ 1.0,2.0 });
	RNRElement<2>		coefficients({ 1.0,1.0 });
	RNRObjectivePtr<2>	objective = paraboloidTestFunction<RNRElement<2>>(offset, coefficients);
	double tolerance = 1e-8;

	// Direction 1
    RNRElement<2>       initialPoint({-2.0,5.0});
    JacobSearch<2>      solver(objective, initialPoint);

    for (size_t i=0; i<2; ++i)
    {
        CIE_TEST_REQUIRE_NOTHROW( solver.step() );
    }

    CIE_TEST_CHECK( std::abs(solver.getSolution().getData()[0] - offset.getData()[0])    < tolerance );
    CIE_TEST_CHECK( std::abs(solver.getSolution().getData()[1] - offset.getData()[1])    < tolerance );
    CIE_TEST_CHECK( std::abs(solver.getSolution().getObjective())						< tolerance );
}


CIE_TEST_CASE( "Jacob search - parabloid_2", "[jacobsearch]" )
{
	// Objective definition
	RNRElement<2>		offset({ 1.0,2.0 });
	RNRElement<2>		coefficients({ 1.0,1.0 });
	RNRObjectivePtr<2>	objective = paraboloidTestFunction<RNRElement<2>>(offset, coefficients);
	double tolerance = 1e-8;

	// Direction 2
	RNRElement<2>       initialPoint({ 2.0,-5.0 });
	JacobSearch<2>      solver(objective, initialPoint);

	for (size_t i = 0; i < 2; ++i)
	{
		CIE_TEST_REQUIRE_NOTHROW(solver.step());
	}

	CIE_TEST_CHECK(std::abs(solver.getSolution().getData()[0] - offset.getData()[0]) < tolerance);
	CIE_TEST_CHECK(std::abs(solver.getSolution().getData()[1] - offset.getData()[1]) < tolerance);
	CIE_TEST_CHECK(std::abs(solver.getSolution().getObjective()) < tolerance);
}


CIE_TEST_CASE( "Jacob search - rosenbrock", "[jacobsearch]" )
{
	// Objective definition
	RNRObjectivePtr<2>	objective = rosenbrockTestFunction();

	// Solver setup
	RNRElement<2>       initialPoint({ 2.0,-5.0 });
	JacobSearch<2>      solver(objective, initialPoint);

	// Iteration
	for (size_t i = 0; i < 15; ++i)
	{
		CIE_TEST_REQUIRE_NOTHROW(solver.step());
		//std::cout << solver.getSolution().getData()[0] << ",\t" << solver.getSolution().getData()[1] << ":\t" << solver.getSolution().getObjective() << std::endl;
	}
	//std::cout << "\n";
}




} // namespace cie::opt