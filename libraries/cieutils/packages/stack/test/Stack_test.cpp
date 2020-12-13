// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/stack/inc/Stack.hpp"


namespace cie::utils {


TEST_CASE( "Stack", "[stack]" )
{
    CIE_TEST_CASE_INIT( "Stack" )

    Stack<Size> stack( 0, 5 );

    for ( Size i=0; i<3; ++i )
        stack.push_back( i );

    CHECK( stack.back() == 2 );

    for ( Size i=0; i<3; ++i )
        CHECK( stack[i] == i );

    for ( Size i=3; i<12; ++i )
        stack.push_back(i);

    for ( Size i=7; i<12; ++i )
        CHECK( i == stack[i] );

    CHECK( stack.back() == 11 );
}


} // namespace cie::utils