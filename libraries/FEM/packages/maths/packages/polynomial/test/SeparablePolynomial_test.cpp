// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/polynomial/inc/SeparablePolynomial.hpp"


namespace cie::fem::maths {


CIE_TEST_CASE( "SeparablePolynomial", "[maths]" )
{
    CIE_TEST_CASE_INIT( "SeparablePolynomial" )

    using Univariate = UnivariatePolynomial<double>;
    using Polynomial = SeparablePolynomial<2,double>;
    using PointType = Kernel<2,double>::point_type;

    // Test polynomial: x^2 * (1 + y)
    Univariate px( Univariate::coefficient_container {0, 0, 1} );
    Univariate py( Univariate::coefficient_container {1, 1} );

    // Check construction
    CIE_TEST_REQUIRE_NOTHROW( Polynomial( Polynomial::univariate_container {px,py} ) );
    Polynomial polynomial( Polynomial::univariate_container {px,py} );

    // Check values
    CIE_TEST_REQUIRE_NOTHROW( polynomial( PointType {0,0} ) );
    CIE_TEST_CHECK( polynomial( PointType { 0, 0 } ) == Approx(0.0) );
    CIE_TEST_CHECK( polynomial( PointType { 2, 0 } ) == Approx(4.0) );
    CIE_TEST_CHECK( polynomial( PointType { 0, 2 } ) == Approx(0.0) );
    CIE_TEST_CHECK( polynomial( PointType { 2, 2 } ) == Approx(12.0) );
    CIE_TEST_CHECK( polynomial( PointType { -2, 0 } ) == Approx(4.0) );
    CIE_TEST_CHECK( polynomial( PointType { 0, -2 } ) == Approx(0.0) );
    CIE_TEST_CHECK( polynomial( PointType { -2, -2 } ) == Approx(-4.0) );

    CIE_TEST_CHECK( polynomial( PointType { 2, 1 } ) == Approx(8.0) );
    CIE_TEST_CHECK( polynomial( PointType { 1, 2 } ) == Approx(3.0) );
    CIE_TEST_CHECK( polynomial( PointType { -2, 1 } ) == Approx(8.0) );
    CIE_TEST_CHECK( polynomial( PointType { 1, -2 } ) == Approx(-1.0) );

    // Check derivative construction
    CIE_TEST_REQUIRE_NOTHROW( polynomial.derivative() );
    auto derivative = polynomial.derivative();

    // Check derivative values
    auto gradient = derivative( PointType { 0, 0 } );
    CIE_TEST_REQUIRE( gradient.size() == 2 );
    CIE_TEST_CHECK( gradient[0] == Approx(0.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(0.0) );

    gradient = derivative( PointType { 2, 0 } );
    CIE_TEST_CHECK( gradient[0] == Approx(4.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(4.0) );

    gradient = derivative( PointType { 0, 2 } );
    CIE_TEST_CHECK( gradient[0] == Approx(0.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(0.0) );

    gradient = derivative( PointType { 2, 2 } );
    CIE_TEST_CHECK( gradient[0] == Approx(12.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(4.0) );

    gradient = derivative( PointType { -2, 0 } );
    CIE_TEST_CHECK( gradient[0] == Approx(-4.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(4.0) );

    gradient = derivative( PointType { 0, -2 } );
    CIE_TEST_CHECK( gradient[0] == Approx(0.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(0.0) );

    gradient = derivative( PointType { -2, -2 } );
    CIE_TEST_CHECK( gradient[0] == Approx(4.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(4.0) );

    gradient = derivative( PointType { 2, 1 } );
    CIE_TEST_CHECK( gradient[0] == Approx(8.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(4.0) );

    gradient = derivative( PointType { 1, 2 } );
    CIE_TEST_CHECK( gradient[0] == Approx(6.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(1.0) );

    gradient = derivative( PointType { -2, 1 } );
    CIE_TEST_CHECK( gradient[0] == Approx(-8.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(4.0) );

    gradient = derivative( PointType { 1, -2 } );
    CIE_TEST_CHECK( gradient[0] == Approx(-2.0) );
    CIE_TEST_CHECK( gradient[1] == Approx(1.0) );

    // Check hessian construction
    CIE_TEST_REQUIRE_NOTHROW( derivative.derivative() );
    auto secondDerivative = derivative.derivative();

    // Check hessian values
    CIE_TEST_REQUIRE_NOTHROW( secondDerivative( PointType {0, 0} ) );
    auto hessian = secondDerivative( PointType {0, 0} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( 2.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( 0.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( 0.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );

    hessian = secondDerivative( PointType {2, 0} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( 2.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( 4.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( 4.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );

    hessian = secondDerivative( PointType {0, 2} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( 6.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( 0.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( 0.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );

    hessian = secondDerivative( PointType {2, 2} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( 6.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( 4.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( 4.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );

    hessian = secondDerivative( PointType {-2, 0} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( 2.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( -4.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( -4.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );

    hessian = secondDerivative( PointType {0, -2} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( -2.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( 0.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( 0.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );

    hessian = secondDerivative( PointType {2, 1} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( 4.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( 4.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( 4.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );

    hessian = secondDerivative( PointType {1, 2} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( 6.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( 2.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( 2.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );

    hessian = secondDerivative( PointType {-2, 1} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( 4.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( -4.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( -4.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );

    hessian = secondDerivative( PointType {1, -2} );
    CIE_TEST_CHECK( hessian(0,0) == Approx( -2.0 ) );
    CIE_TEST_CHECK( hessian(1,0) == Approx( 2.0 ) );
    CIE_TEST_CHECK( hessian(0,1) == Approx( 2.0 ) );
    CIE_TEST_CHECK( hessian(1,1) == Approx( 0.0 ) );
}


} // namespace cie::fem::maths