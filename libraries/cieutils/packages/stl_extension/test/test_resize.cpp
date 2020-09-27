// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "cieutils/inc/cieutils/stl_extension.hpp"

// --- STL Includes ---
#include <vector>
#include <array>


namespace cie {


TEST_CASE( "Concept utilities", "[concepts]" )
{
    std::vector<int>    vector(1);
    std::array<int,3>   array;

    CHECK_NOTHROW( utils::resize(vector,3) );
    CHECK( vector.size() == 3 );

    CHECK_NOTHROW( utils::resize(array, 3) );
    CHECK( array.size() == 3 );
}


}