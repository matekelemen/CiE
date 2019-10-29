#include "catch.hpp"
#include "../../decomposition/inc/eigendecomposition.hpp"

namespace linalg {

TEST_CASE("Principal component - power iteration"){

    // Diagonalizable
    Matrix matrix(
        {
            6.0,    3.0,    2.0,
            0.0,    12.0,   4.0,
            0.0,    0.0,    18.0
        }, 3
    );
    Vector principal = {1.0, 2.0, 3.0};
    Vector result;
    double tolerance = 1e-15;

    REQUIRE_NOTHROW( result = principalComponent(matrix, tolerance) );
    normalize(principal);

    REQUIRE( result.size() == principal.size() );
    for (size_t i=0; i<result.size(); ++i){
        CHECK( abs(result[i]-principal[i]) < tolerance );
    }

}

}