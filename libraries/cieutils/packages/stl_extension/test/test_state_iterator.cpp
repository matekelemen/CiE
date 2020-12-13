// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/stl_extension/inc/state_iterator.hpp"

// --- STL Includes ---
#include <vector>
#include <array>
#include <iostream>


namespace cie::utils {


TEST_CASE( "StateIterator", "[stl_extension]" )
{
    typedef std::vector<int>                            SubContainer;
    typedef std::array<std::vector<int>,2>              Container;

    const Container container = 
    {
        SubContainer({ 0, 1, 2 }),
        SubContainer({ 3, 4 })
    };

    REQUIRE_NOTHROW( makeStateIterator( container.begin(), container.end() ) );
    REQUIRE_NOTHROW( makeStateIterator( container ) );

    auto state = makeStateIterator(container);
    //StateIterator<typename Container::const_iterator> state(container);
    REQUIRE_NOTHROW( *state );
    REQUIRE_NOTHROW( ++state );
    REQUIRE_NOTHROW( state.reset() );

    // state 0 -> 0,3
    REQUIRE( (*state).size() == container.size() );
    CHECK( *(*state)[0] == 0 );     CHECK( *(*state)[1] == 3 );
    CHECK_NOTHROW( ++state );

    // state 0 -> 1,3
    REQUIRE( (*state).size() == container.size() );
    CHECK( *(*state)[0] == 1 );     CHECK( *(*state)[1] == 3 );
    CHECK_NOTHROW( ++state );

    // state 0 -> 2,3
    REQUIRE( (*state).size() == container.size() );
    CHECK( *(*state)[0] == 2 );     CHECK( *(*state)[1] == 3 );
    CHECK_NOTHROW( ++state );

    // state 0 -> 0,4
    REQUIRE( (*state).size() == container.size() );
    CHECK( *(*state)[0] == 0 );     CHECK( *(*state)[1] == 4 );
    CHECK_NOTHROW( ++state );

    // state 0 -> 1,4
    REQUIRE( (*state).size() == container.size() );
    CHECK( *(*state)[0] == 1 );     CHECK( *(*state)[1] == 4 );
    CHECK_NOTHROW( ++state );

    // state 0 -> 2,4
    REQUIRE( (*state).size() == container.size() );
    CHECK( *(*state)[0] == 2 );     CHECK( *(*state)[1] == 4 );
    CHECK_NOTHROW( ++state );

} // TEST_CASE( "StateIterator")


TEST_CASE( "InternalStateIterator", "[stl_extension]" )
{
    const Size dimension        = 2;
    std::array<int,3> container = { 0, 1, 2 };
    auto state                  = makeInternalStateIterator( container, dimension );

    for (Size i=0; i<container.size(); ++i)
        for (Size j=0; j<container.size(); ++j)
        {
            REQUIRE( (*state).size() == dimension );
            CHECK( *(*state)[0] == container[j] );     CHECK( *(*state)[1] == container[i] );
            CHECK_NOTHROW( ++state );
        }
} // TEST_CASE( "InternalStateIterator")


} // namespace cie::utils