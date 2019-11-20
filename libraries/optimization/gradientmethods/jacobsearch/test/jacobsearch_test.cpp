#include "catch.hpp"
#include "../inc/jacobsearch.hpp"

namespace cie {
namespace opt {


double jacobSearchTestFunction(const RNRElement<2>& point)
{
    return 0.0;
}


TEST_CASE("Jacob search")
{
    RNRObjectivePtr<2>  objective       = jacobSearchTestFunction;
    RNRElement<2>       initialPoint({0.0,0.0});
    JacobSearch<2>      solver(objective, initialPoint);
}


}
}