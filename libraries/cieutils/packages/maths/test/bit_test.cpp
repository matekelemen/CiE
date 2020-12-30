// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/maths/inc/bit.hpp"


namespace cie::utils {


CIE_TEST_CASE( "flipBit", "[maths]" )
{
    CIE_TEST_CASE_INIT( "flipBit" )

    Size test = 0;

    CIE_TEST_CHECK_NOTHROW( test = flipBit(test, 3) );
    CIE_TEST_CHECK( test == 8 );

    CIE_TEST_CHECK_NOTHROW( test = flipBit(test, 10) );
    CIE_TEST_CHECK( test == 8 + 1024 );

    CIE_TEST_CHECK_NOTHROW( test = flipBit(test, 3) );
    CIE_TEST_CHECK( test == 1024 );
}


} // namespace cie::utils