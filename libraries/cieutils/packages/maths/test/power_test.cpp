// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/power.hpp"
#include "../../types/inc/types.hpp"


namespace cie {


TEST_CASE( "intPow", "[maths]" )
{
    typedef Size    NT;

    NT base     = 2;
    NT check    = 1;
    for (Size i=0; i<17; ++i)
    {
        REQUIRE_NOTHROW( intPow(base,i) );
        CHECK( intPow(base,i) == check );
        check *= base;
    }

    base        = 13;
    check       = 1;
    for (Size i=0; i<5; ++i)
    {
        REQUIRE_NOTHROW( intPow(base,i) );
        CHECK( intPow(base,i) == check );
        check *= base;
    }
}


} // namespace cie