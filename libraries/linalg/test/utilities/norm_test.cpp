#include "catch.hpp"
#include "../../utilities/inc/linalgutilities.hpp"

namespace linalg {

TEST_CASE("Norms and normalization")
{
    Vector vector = {0.0, 0.0, 0.0, 0.0, 0.0};
    Matrix matrix( {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 2 );

    CHECK( norm(vector) == Approx(0.0) );
    CHECK( norm(matrix) == Approx(0.0) );

    REQUIRE_THROWS( normalize(vector) );
    REQUIRE_THROWS( normalize(matrix) );

    vector = { -1.0, -4.0, 8.0 };
    matrix = Matrix( {1.0, -3.0, 1.0, -5.0}, 2 );

    CHECK( norm(vector) == Approx(9.0) );
    CHECK( norm(matrix) == Approx(6.0) );

    REQUIRE_NOTHROW( normalize(vector) );
    REQUIRE_NOTHROW( normalize(matrix) );
    CHECK( vector[0] == Approx( -1/9.0 ) );
    CHECK( vector[1] == Approx( -4/9.0 ) );
    CHECK( vector[2] == Approx( 8/9.0 ) );
    CHECK( matrix(0,0) == Approx( 1/6.0 ) );
    CHECK( matrix(0,1) == Approx( -3/6.0 ) );
    CHECK( matrix(1,0) == Approx( 1/6.0 ) );
    CHECK( matrix(1,1) == Approx( -5/6.0 ) );
}

}