// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/cell.hpp"


namespace cie::csg {


TEST_CASE( "CubeCell", "[trees]" )
{
    using TestCell = CubeCell<2,Double>;

    REQUIRE_NOTHROW( TestCell( DoubleArray<2>({1.0,1.0}), 2.0) );
    TestCell cell( DoubleArray<2>({2.0,2.0}), 2.0 );

    REQUIRE_NOTHROW( cell.isInside( DoubleArray<2>({-1.0,0.0}) ) );
    CHECK( cell.isInside( DoubleArray<2>({-1.0,0.0}) ) == false );
    CHECK( cell.isInside( cell.base()) == true );

    REQUIRE_NOTHROW( cell.split() );
    REQUIRE( cell.children().size() == 4 );

    CHECK( std::abs(cell.children()[0]->length() - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[0]->base()[0] - 2.0) < 1e-16 );
    CHECK( std::abs(cell.children()[0]->base()[1] - 2.0) < 1e-16 );

    CHECK( std::abs(cell.children()[1]->length() - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[1]->base()[0] - 3.0) < 1e-16 );
    CHECK( std::abs(cell.children()[1]->base()[1] - 2.0) < 1e-16 );

    CHECK( std::abs(cell.children()[2]->length() - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[2]->base()[0] - 2.0) < 1e-16 );
    CHECK( std::abs(cell.children()[2]->base()[1] - 3.0) < 1e-16 );

    CHECK( std::abs(cell.children()[3]->length() - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[3]->base()[0] - 3.0) < 1e-16 );
    CHECK( std::abs(cell.children()[3]->base()[1] - 3.0) < 1e-16 );
}


TEST_CASE( "BoxCell", "[trees]" )
{
    using TestCell = BoxCell<2,Double>;

    REQUIRE_NOTHROW( TestCell( DoubleArray<2>({1.0,1.0}), DoubleArray<2>({2.0,2.0}) ) );
    TestCell cell( DoubleArray<2>({2.0,2.0}), DoubleArray<2>({4.0,2.0}) );

    REQUIRE_NOTHROW( cell.isInside( DoubleArray<2>({0.0,0.0}) ) );
    CHECK( cell.isInside( DoubleArray<2>({0.0,0.0}) ) == false );
    CHECK( cell.isInside( cell.center()) == true );

    #ifdef CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS
    REQUIRE_THROWS( cell.split( DoubleArray<2>({0.0,0.0}) ) );
    #endif
    REQUIRE_NOTHROW( cell.split(DoubleArray<2>({1.0,2.0})) );
    REQUIRE( cell.children().size() == 4 );

    CHECK( std::abs(cell.children()[0]->lengths()[0] - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[0]->lengths()[1] - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[0]->center()[0] - 0.5) < 1e-16 );
    CHECK( std::abs(cell.children()[0]->center()[1] - 1.5) < 1e-16 );

    CHECK( std::abs(cell.children()[1]->lengths()[0] - 3.0) < 1e-16 );
    CHECK( std::abs(cell.children()[1]->lengths()[1] - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[1]->center()[0] - 2.5) < 1e-16 );
    CHECK( std::abs(cell.children()[1]->center()[1] - 1.5) < 1e-16 );

    CHECK( std::abs(cell.children()[2]->lengths()[0] - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[2]->lengths()[1] - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[2]->center()[0] - 0.5) < 1e-16 );
    CHECK( std::abs(cell.children()[2]->center()[1] - 2.5) < 1e-16 );

    CHECK( std::abs(cell.children()[3]->lengths()[0] - 3.0) < 1e-16 );
    CHECK( std::abs(cell.children()[3]->lengths()[1] - 1.0) < 1e-16 );
    CHECK( std::abs(cell.children()[3]->center()[0] - 2.5) < 1e-16 );
    CHECK( std::abs(cell.children()[3]->center()[1] - 2.5) < 1e-16 );
}

}