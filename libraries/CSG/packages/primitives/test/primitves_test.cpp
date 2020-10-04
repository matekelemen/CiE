// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Linalg Includes ---
#include "linalg/packages/types/inc/vectortypes.hpp"
#include "linalg/packages/types/inc/arraytypes.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/primitives.hpp"

namespace cie::csg {

namespace boolean {


TEST_CASE( "boolean::CSGCube", "[primitives]" )
{
    CIE_TEST_CASE_INIT( "boolean::CSGCube" )

    const Size dimension = 2;

    REQUIRE_NOTHROW( CSGCube<dimension,Double>(DoubleArray<dimension>({10.0,20.0}),2.0) );
    CSGCube<dimension,Double> cube( DoubleArray<dimension>({10.0,20.0}), 
                                    2.0 );
    CHECK( CSGCube<dimension,Double>::dimension == dimension );

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
    CIE_TEST_CASE_INIT( "boolean::CSGBox" )

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


} // namespace boolean


} // namespace cie::csg