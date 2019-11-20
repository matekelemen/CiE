#include "catch.hpp"
#include "../inc/octreeutils.hpp"


namespace cie {
namespace csg {


TEST_CASE("Base converters"){
    // Base 2
    uint test  = 7;
    std::array<bool,3> test2 = base2(test);
    for (uint i=0; i<3; ++i){
        CHECK( test2[i] == true );
    }

    test    = 3;
    test2   = base2(test);
    CHECK( test2[0] == true );
    CHECK( test2[1] == true );
    CHECK( test2[2] == false );

    // Base 3
    test  = 23;
    std::array<uint,3> test3 = base3(test);
    CHECK( test3[0] == 2 );
    CHECK( test3[1] == 1 );
    CHECK( test3[2] == 2 );

    test    = 13;
    test3   = base3(test);
    CHECK( test3[0] == 1 );
    CHECK( test3[1] == 1 );
    CHECK( test3[2] == 1 );
}



}
}