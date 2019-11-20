#include "catch.hpp"
#include "../inc/element.hpp"

namespace cie {
namespace opt {

TEST_CASE("Element - constructors")
{
    RRElement siso(1.0);
    CHECK( siso.getData() == Approx(1.0) );
    CHECK( siso.getObjective() == Approx(0.0) );

    RNRElement<3> miso( {0.0,1.0,2.0} );
    for (size_t i=0; i<3; ++i)
    {
        CHECK( miso.getData()[i] == Approx( (double)i ) );
    }
}

}
}