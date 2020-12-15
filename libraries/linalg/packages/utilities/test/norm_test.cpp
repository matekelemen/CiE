// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "linalg/packages/types/inc/matrix.hpp"
#include "linalg/packages/types/inc/vectortypes.hpp"
#include "linalg/packages/types/inc/typeoperations.hpp"

namespace cie::linalg {

CIE_TEST_CASE( "Norms and normalization", "[utilities]" )
{
    DoubleVector vector = {0.0, 0.0, 0.0, 0.0, 0.0};
    Matrix<Double> matrix( {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 2 );

    CIE_TEST_CHECK( norm<DoubleVector>(vector) == Approx(0.0) );
    CIE_TEST_CHECK( norm<Matrix<Double>>(matrix) == Approx(0.0) );

    CIE_TEST_REQUIRE_THROWS( normalize(vector) );
    CIE_TEST_REQUIRE_THROWS( normalize(matrix) );

    vector = { -1.0, -4.0, 8.0 };
    matrix = Matrix<Double>( {1.0, -3.0, 1.0, -5.0}, 2 );

    CIE_TEST_CHECK( norm<DoubleVector>(vector) == Approx(9.0) );
    CIE_TEST_CHECK( norm<Matrix<Double>>(matrix) == Approx(6.0) );

    CIE_TEST_REQUIRE_NOTHROW( normalize(vector) );
    CIE_TEST_REQUIRE_NOTHROW( normalize(matrix) );
    CIE_TEST_CHECK( vector[0] == Approx( -1/9.0 ) );
    CIE_TEST_CHECK( vector[1] == Approx( -4/9.0 ) );
    CIE_TEST_CHECK( vector[2] == Approx( 8/9.0 ) );
    CIE_TEST_CHECK( matrix(0,0) == Approx( 1/6.0 ) );
    CIE_TEST_CHECK( matrix(0,1) == Approx( -3/6.0 ) );
    CIE_TEST_CHECK( matrix(1,0) == Approx( 1/6.0 ) );
    CIE_TEST_CHECK( matrix(1,1) == Approx( -5/6.0 ) );
}

} // namespace cie::linalg