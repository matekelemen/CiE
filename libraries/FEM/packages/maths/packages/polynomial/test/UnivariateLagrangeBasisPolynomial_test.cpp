// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/polynomial/inc/UnivariateLagrangeBasisPolynomial.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::fem::maths {


CIE_TEST_CASE( "UnivariateLagrangeBasisPolynomial", "[maths]" )
{
    CIE_TEST_CASE_INIT( "UnivariateLagrangeBasisPolynomial" )

    using Polynomial = UnivariateLagrangeBasisPolynomial<double>;

    std::vector<double> nodes { 0.0, 1.0/3.0, 2.0/3.0, 1.0 };

    CIE_TEST_REQUIRE_NOTHROW( Polynomial(nodes, 0) );
    Polynomial polynomial( nodes, 0 );

    CIE_TEST_CHECK( polynomial(0.0) == Approx(1.0) );
    CIE_TEST_CHECK( polynomial(1.0/3.0) == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( polynomial(2.0/3.0) == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( polynomial(1.0) == Approx(0.0).margin(1e-14) );

    CIE_TEST_CHECK_NOTHROW( polynomial = Polynomial(nodes, 1) );
    CIE_TEST_CHECK( polynomial(0.0) == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( polynomial(1.0/3.0) == Approx(1.0) );
    CIE_TEST_CHECK( polynomial(2.0/3.0) == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( polynomial(1.0) == Approx(0.0).margin(1e-14) );

    CIE_TEST_CHECK_NOTHROW( polynomial = Polynomial(nodes, 2) );
    CIE_TEST_CHECK( polynomial(0.0) == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( polynomial(1.0/3.0) == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( polynomial(2.0/3.0) == Approx(1.0) );
    CIE_TEST_CHECK( polynomial(1.0) == Approx(0.0).margin(1e-14) );

    CIE_TEST_CHECK_NOTHROW( polynomial = Polynomial(nodes, 3) );
    CIE_TEST_CHECK( polynomial(0.0) == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( polynomial(1.0/3.0) == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( polynomial(2.0/3.0) == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( polynomial(1.0) == Approx(1.0) );
}


} // namespace cie::fem::maths