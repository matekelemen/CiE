// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "optimization/packages/representation/inc/element.hpp"

namespace cie::opt {

CIE_TEST_CASE( "Element - constructors", "[representation]" )
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

} // namespace cie::opt