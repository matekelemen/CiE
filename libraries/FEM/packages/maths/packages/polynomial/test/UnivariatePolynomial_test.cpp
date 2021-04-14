// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/polynomial/inc/UnivariatePolynomial.hpp"


namespace cie::fem::maths {


CIE_TEST_CASE( "UnivariatePolynomial", "[maths]" )
{
    CIE_TEST_CASE_INIT( "UnivariatePolynomial" )

    using Polynomial = UnivariatePolynomial<double>;

    auto checkPolynomial = []( const UnivariateScalarFunction<double>& r_polynomial )
    {
        CIE_TEST_CHECK_NOTHROW( r_polynomial( 0 ) );
        CIE_TEST_CHECK( r_polynomial(0) == Approx(0.0) );
        CIE_TEST_CHECK( r_polynomial(1) == Approx(1.0) );
        CIE_TEST_CHECK( r_polynomial(2) == Approx(4.0) );
        CIE_TEST_CHECK( r_polynomial(-1) == Approx(1.0) );
        CIE_TEST_CHECK( r_polynomial(-2) == Approx(4.0) );
    };

    auto checkDerivative = []( const UnivariateScalarFunction<double>& r_derivative )
    {
        CIE_TEST_CHECK_NOTHROW( r_derivative( 0 ) );
        CIE_TEST_CHECK( r_derivative(0) == Approx(0.0) );
        CIE_TEST_CHECK( r_derivative(1) == Approx(2.0) );
        CIE_TEST_CHECK( r_derivative(2) == Approx(4.0) );
        CIE_TEST_CHECK( r_derivative(-1) == Approx(-2.0) );
        CIE_TEST_CHECK( r_derivative(-2) == Approx(-4.0) );
    };

    Polynomial::coefficient_container coefficients { 0, 0, 1 };

    CIE_TEST_REQUIRE_NOTHROW( Polynomial(coefficients) );
    Polynomial polynomial( coefficients );

    checkPolynomial( polynomial );

    {
        CIE_TEST_REQUIRE_NOTHROW( Polynomial( {0, 0, 1} ) );
        Polynomial copy( Polynomial({0, 0, 1}) );
        checkPolynomial( copy );
    }

    {
        CIE_TEST_REQUIRE_NOTHROW( Polynomial( polynomial ) );
        Polynomial copy( polynomial );
        checkPolynomial( copy );
    }

    CIE_TEST_REQUIRE_NOTHROW( polynomial.derivative() );
    auto p_derivative = polynomial.derivative();
    checkDerivative( *p_derivative );

    {
        CIE_TEST_REQUIRE_NOTHROW( Polynomial() );
        Polynomial defaultPolynomial;
        CIE_TEST_REQUIRE_NOTHROW( defaultPolynomial(0) );
        CIE_TEST_CHECK( defaultPolynomial(0) == Approx(0.0) );
        CIE_TEST_CHECK( defaultPolynomial(2) == Approx(0.0) );
        CIE_TEST_CHECK( defaultPolynomial(-2) == Approx(0.0) );
    }

    // Construction via initializer list
    CIE_TEST_CHECK_NOTHROW( Polynomial({0, 0, 1}) );
}


} // namespace cie::fem::maths