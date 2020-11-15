// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "ciegl/packages/file/inc/STLPart.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <iostream>


namespace cie::gl {


TEST_CASE( "STLPart", "[file]" )
{
    CIE_TEST_CASE_INIT( "STLPart" )

    REQUIRE_NOTHROW( STLPart() );
    STLPart part;


    REQUIRE_NOTHROW( part = STLPart( SOURCE_PATH + "/libraries/ciegl/data/geometry/stl/Bunny.stl" ) );
}


} // namespace cie::gl