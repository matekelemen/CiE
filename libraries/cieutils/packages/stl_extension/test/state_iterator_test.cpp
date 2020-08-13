// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/state_iterator.hpp"

// --- STL Includes ---
#include <vector>
#include <array>
#include <iostream>


namespace cie::utils {


TEST_CASE( "StateIterator", "[stl_extension]" )
{
    typedef std::vector<int>                SubContainer;
    typedef std::array<std::vector<int>,2>  Container;
    typedef StateIterator<Container>        State;

    Container container = 
    {
        SubContainer({ 0, 1, 2 }),
        SubContainer({ 3, 4 })
    };

    State state(container);
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


} // namespace cie::utils