// --- External Includes ---
#include "catch.hpp"

// --- Linalg Includes ---
#include <linalg/types.hpp>

// --- Internal Includes ---
#include "../inc/primitives.hpp"

namespace cie::csg {

namespace boolean {


TEST_CASE( "boolean::CSGCube", "[primitives]" )
{
    const Size dimension = 2;
    CSGCube<dimension,Double> cube( DoubleArray<dimension>({10.0,20.0}), 
                                    2.0 );
    DoubleArray<dimension> point = {0.0, 0.0};

    REQUIRE_NOTHROW( cube.evaluate(point) );
    CHECK( cube.evaluate(point) == false );

    REQUIRE_NOTHROW( cube.evaluate(cube.base()) );
    CHECK( cube.evaluate( cube.base() ) == true );

    REQUIRE_NOTHROW( cube.evaluate( DoubleVector({11.999,21.999}) ) );
    CHECK( cube.evaluate( DoubleVector({11.999,21.999}) ) == true );
}


TEST_CASE( "boolean::CSGBox", "[primitives]" )
{
    const Size dimension = 2;
    CSGBox<dimension,Double> box(   DoubleArray<dimension>({10.0,20.0}), 
                                    DoubleArray<dimension>({1.0,2.0}) );
    DoubleArray<dimension> point = {0.0, 0.0};

    REQUIRE_NOTHROW( box.evaluate(point) );
    CHECK( box.evaluate(point) == false );

    REQUIRE_NOTHROW( box.evaluate(box.base()) );
    CHECK( box.evaluate( box.base() ) == true );

    REQUIRE_NOTHROW( box.evaluate( DoubleVector({10.999999,21.99999}) ) );
    CHECK( box.evaluate( DoubleVector({10.999999,21.99999}) ) == true );
}


}


}