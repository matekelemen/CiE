#include "catch.hpp"
#include "../inc/jacobsearch.hpp"
#include "linalgtypes.hpp"
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


TEST_CASE("Jacob search 1")
{
    RNRObjectivePtr<2>  objective       = jacobSearchTestFunction;
    RNRElement<2>       initialPoint({-2.0,5.0});
    JacobSearch<2>      solver(objective, initialPoint);
    double tolerance    = 1e-8;

    for (size_t i=0; i<2; ++i)
    {
        REQUIRE_NOTHROW( solver.step() );
    }

    CHECK( std::abs(solver.getSolution().getData()[0] - 1.0)    < tolerance );
    CHECK( std::abs(solver.getSolution().getData()[1] - 1.0)    < tolerance );
    CHECK( std::abs(solver.getSolution().getObjective())        < tolerance );
}


TEST_CASE("Jacob search 2")
{
    // Just like "Jacob search 1, but from the opposite direction"
    RNRObjectivePtr<2>  objective       = jacobSearchTestFunction;
    RNRElement<2>       initialPoint({2.0,-5.0});
    JacobSearch<2>      solver(objective, initialPoint);
    double tolerance    = 1e-8;

    for (size_t i=0; i<2; ++i)
    {
        REQUIRE_NOTHROW( solver.step() );
    }

    CHECK( std::abs(solver.getSolution().getData()[0] - 1.0)    < tolerance );
    CHECK( std::abs(solver.getSolution().getData()[1] - 1.0)    < tolerance );
    CHECK( std::abs(solver.getSolution().getObjective())        < tolerance );
}


}
}