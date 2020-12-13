// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/file/inc/STLPart.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <iostream>


namespace cie::gl {


TEST_CASE( "STLPart", "[file]" )
{
    CIE_TEST_CASE_INIT( "STLPart" )

    PartPtr p_part;

    REQUIRE_NOTHROW( p_part.reset( 
        new STLPart( SOURCE_PATH / "libraries/ciegl/data/geometry/stl/Bunny.stl" ))
    );
}


} // namespace cie::gl