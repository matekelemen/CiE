// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/cell.hpp"


namespace cie::csg {

TEST_CASE( "BoxCell", "[trees]" )
{
    using TestBox = BoxCell<2,Double>;
    REQUIRE_NOTHROW( TestBox( DoubleArray<2>({1.0,1.0}), DoubleArray<2>({2.0,2.0}) ) );

    TestBox box( DoubleArray<2>({2.0,2.0}), DoubleArray<2>({4.0,2.0}) );

    REQUIRE_NOTHROW( box.isInside( DoubleArray<2>({0.0,0.0}) ) );
    CHECK( box.isInside( DoubleArray<2>({0.0,0.0}) ) == false );
    CHECK( box.isInside( box.center()) == true );

    #ifdef CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS
    REQUIRE_THROWS( box.split( DoubleArray<2>({0.0,0.0}) ) );
    #endif
    REQUIRE_NOTHROW( box.split(DoubleArray<2>({1.0,2.0})) );

    CHECK( std::abs(box.children()[0]->lengths()[0] - 1.0) < 1e-16 );
    CHECK( std::abs(box.children()[0]->lengths()[1] - 1.0) < 1e-16 );
    CHECK( std::abs(box.children()[0]->center()[0] - 0.5) < 1e-16 );
    CHECK( std::abs(box.children()[0]->center()[1] - 1.5) < 1e-16 );

    CHECK( std::abs(box.children()[1]->lengths()[0] - 3.0) < 1e-16 );
    CHECK( std::abs(box.children()[1]->lengths()[1] - 1.0) < 1e-16 );
    CHECK( std::abs(box.children()[1]->center()[0] - 2.5) < 1e-16 );
    CHECK( std::abs(box.children()[1]->center()[1] - 1.5) < 1e-16 );

    CHECK( std::abs(box.children()[2]->lengths()[0] - 1.0) < 1e-16 );
    CHECK( std::abs(box.children()[2]->lengths()[1] - 1.0) < 1e-16 );
    CHECK( std::abs(box.children()[2]->center()[0] - 0.5) < 1e-16 );
    CHECK( std::abs(box.children()[2]->center()[1] - 2.5) < 1e-16 );

    CHECK( std::abs(box.children()[3]->lengths()[0] - 3.0) < 1e-16 );
    CHECK( std::abs(box.children()[3]->lengths()[1] - 1.0) < 1e-16 );
    CHECK( std::abs(box.children()[3]->center()[0] - 2.5) < 1e-16 );
    CHECK( std::abs(box.children()[3]->center()[1] - 2.5) < 1e-16 );
}

}