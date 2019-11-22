#include "catch.hpp"
#include "../inc/jacobsearch.hpp"
#include "linalgtypes.hpp"
#include "../../solver/inc/testfunctions.hpp"
#include <iostream>

namespace cie {
namespace opt {


double jacobSearchTestFunction(const RNRElement<2>& point)
{
    double out = 0.0;
    for (size_t i=0; i<point.getData().size(); ++i)
    {
        out += (point.getData()[i]-1.0) * (point.getData()[i]-1.0);
    }
    return out;
}


TEST_CASE("Jacob search - paraboloid_1")
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
        REQUIRE_NOTHROW( solver.step() );
    }

    CHECK( std::abs(solver.getSolution().getData()[0] - offset.getData()[0])    < tolerance );
    CHECK( std::abs(solver.getSolution().getData()[1] - offset.getData()[1])    < tolerance );
    CHECK( std::abs(solver.getSolution().getObjective())						< tolerance );
}


TEST_CASE("Jacob search - parabloid_2")
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
		REQUIRE_NOTHROW(solver.step());
	}

	CHECK(std::abs(solver.getSolution().getData()[0] - offset.getData()[0]) < tolerance);
	CHECK(std::abs(solver.getSolution().getData()[1] - offset.getData()[1]) < tolerance);
	CHECK(std::abs(solver.getSolution().getObjective()) < tolerance);
}


TEST_CASE("Jacob search - rosenbrock")
{
	// Objective definition
	RNRObjectivePtr<2>	objective = rosenbrockTestFunction();

	// Solver setup
	RNRElement<2>       initialPoint({ 2.0,-5.0 });
	JacobSearch<2>      solver(objective, initialPoint);

	// Iteration
	for (size_t i = 0; i < 15; ++i)
	{
		REQUIRE_NOTHROW(solver.step());
		std::cout << solver.getSolution().getData()[0] << ",\t" << solver.getSolution().getData()[1] << ":\t" << solver.getSolution().getObjective() << std::endl;
	}
	std::cout << "\n";
}




}
}