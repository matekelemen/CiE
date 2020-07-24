#include "catch.hpp"
#include "../inc/gradientdescent.hpp"
#include "../../solver/inc/testfunctions.hpp"
#include <iostream>

namespace cie::opt {


double parabolaGradientTest(const RNRElement<2>& point)
{
    double out = 0.0;
    for (size_t i=0; i<2; ++i)
    {
        out += (point.getData()[i]-1.0) * (point.getData()[i]-1.0);
    }
    return out;
}


TEST_CASE("Gradient descent")
{
    RNRObjectivePtr<2> parabola = parabolaGradientTest;
    GradientDescent<2> solver(parabola,RNRElement<2>({3.0,2.0}));
}


TEST_CASE("Gradient descent - rosenbrock")
{
	// Objective definition
	RNRObjectivePtr<2>	objective = rosenbrockTestFunction();

	// Solver setup
	RNRElement<2>			initialPoint({ 2.0,-5.0 });
	GradientDescent<2>      solver(objective, initialPoint);

	// Iteration
	for (size_t i = 0; i < 15; ++i)
	{
		REQUIRE_NOTHROW(solver.step());
		//std::cout << solver.getSolution().getData()[0] << ",\t" << solver.getSolution().getData()[1] << ":\t" << solver.getSolution().getObjective() << std::endl;
	}
	//std::cout << "\n";
}

}