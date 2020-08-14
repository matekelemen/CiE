// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/base.hpp"


namespace cie::utils {


TEST_CASE( "base", "[maths]" )
{
    const Size                      NDigits = 4;
    typedef Size                    NT;
    typedef std::array<NT,NDigits>  Array;

    NT base;

    SECTION( "baseN in-place" )
    {
        NT value    = 13;
        base        = 2;
        Array check = { 1, 0, 1, 1 };
        Array test;

        REQUIRE_NOTHROW( baseN( value, base, test ) );
        for (Size i=0; i<check.size(); ++i)
            CHECK( test[i] == check[i] );


        base        = 5;
        check       = { 3, 2, 0, 0 };
        REQUIRE_NOTHROW( baseN( value, base, test ) );
        for (Size i=0; i<check.size(); ++i)
            CHECK( test[i] == check[i] );
    }

    SECTION( "baseN" )
    {
        NT value    = 13;
        base        = 2;
        Array check = { 1, 0, 1, 1 };
        Array test;

        REQUIRE_NOTHROW( test = baseN<NDigits>( value, base ) );
        for (Size i=0; i<check.size(); ++i)
            CHECK( test[i] == check[i] );

        base        = 5;
        check       = { 3, 2, 0, 0 };
        REQUIRE_NOTHROW( test = baseN<NDigits>( value, base ) );
        for (Size i=0; i<check.size(); ++i)
            CHECK( test[i] == check[i] );
    }

    SECTION( "base10" )
    {
        Array value     = { 1, 0, 1, 1 };
        base            = 2;
        NT check        = 13;
        NT test;

        REQUIRE_NOTHROW( test = base10( value, base ) );
        CHECK( test == check );

        base            = 5;
        check           = 151;
        REQUIRE_NOTHROW( test = base10( value, base ) );
        CHECK( test == check );
    }

    SECTION( "reinterpretBase" )
    {
        Array value     = { 1, 0, 1, 1 };
        NT base         = 2;
        NT check        = 13;
        NT test;

        REQUIRE_NOTHROW( test = reinterpretBase( value, base ) );
        CHECK( test == check );

        base            = 5;
        check           = 151;
        REQUIRE_NOTHROW( test = reinterpretBase( value, base ) );
        CHECK( test == check );
    }

} // TEST_CASE baseN


} // namespace cie::utils