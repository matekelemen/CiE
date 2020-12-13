// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "linalg/linalg.hpp"

namespace cie::linalg {

CIE_TEST_CASE( "Principal component - power iteration", "[decomposition]" )
{

    // Diagonalizable
    Matrix matrix(
        {
            6.0,    3.0,    2.0,
            0.0,    12.0,   4.0,
            0.0,    0.0,    18.0
        }, 3
    );
    DoubleVector principal = {1.0, 2.0, 3.0};
    DoubleVector result;
    double tolerance		= 1e-16;
	double checkTolerance	= 1e-15;

    CIE_TEST_REQUIRE_NOTHROW( result = principalComponent(matrix, tolerance) );
    normalize<DoubleVector>(principal);

    CIE_TEST_REQUIRE( result.size() == principal.size() );
    for (size_t i=0; i<result.size(); ++i){
        CIE_TEST_CHECK( abs(result[i]-principal[i]) < checkTolerance);
    }

}

} // namespace cie::linalg