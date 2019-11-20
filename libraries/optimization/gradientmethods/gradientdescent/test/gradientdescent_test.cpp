#include "catch.hpp"
#include "../inc/gradientdescent.hpp"
#include <iostream>

namespace cie {
namespace opt {


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

}
}