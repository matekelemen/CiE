#ifndef OPTIMIZATION_TEST_FUNCTIONS_HPP
#define OPTIMIZATION_TEST_FUNCTIONS_HPP

// --- Internal Includes ---
#include "optimization/packages/objective/inc/objectivefunction.hpp"

namespace cie::opt {


// Simple paraboloid function with offset and scaling - expects RNRElements and RNRVectorElements
template <typename ElementType>
auto paraboloidTestFunction(const ElementType& offset, const ElementType& coefficients)
{
	return [=](const ElementType& element) -> double
	{
		double out = 0.0;
		for (size_t i = 0; i < element.getData().size(); ++i)
		{
			out += coefficients.getData()[i] * (element.getData()[i] - offset.getData()[i]) * (element.getData()[i] - offset.getData()[i]);
		}
		return out;
	};
}


// Rosenbrock function - only for solvers supporting RNRElement<2>
inline auto rosenbrockTestFunction()
{
	return [](const RNRElement<2>& element) -> double
	{
		double x = element.getData()[0];
		double y = element.getData()[1];

		return (1 - x)*(1 - x) + 100 * (y - x * x)*(y - x * x);
	};
}



} // namespace cie::opt


#endif