// --- External Includes ---
#include "catch.hpp"

// --- Linalg Includes ---
#include <linalg/types.hpp>

// --- Internal Includes ---
#include "../inc/primitives.hpp"

namespace cie::csg {

namespace boolean {

TEST_CASE( "boolean::CSGBox", "[primitives]" )
{
    const Size dimension = 2;
    CSGBox<dimension,Double> box(  DoubleArray<dimension>({10.0,20.0}), 
                                DoubleArray<dimension>({1.0,2.0}) );
    DoubleArray<dimension> point = {0.0, 0.0};

    CHECK( box.evaluate(point) == false );
    CHECK( box.evaluate( box.center() ) == true );
    CHECK( box.evaluate( DoubleVector({10.499999,20.99999}) ) == true );
}

}


}