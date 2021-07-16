// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/operation/inc/Sum.hpp"
#include "FEM/packages/maths/inc/SeparableScalarFunction.hpp"
#include "FEM/packages/maths/packages/polynomial/inc/UnivariatePolynomial.hpp"


namespace cie::fem::maths {


CIE_TEST_CASE( "makeSum", "[maths]" )
{
    CIE_TEST_CASE_INIT( "makeSum" )

    using Univariate = UnivariateScalarFunction<double>;

    Univariate::SharedPointer p_p0( new UnivariatePolynomial<double>( {1.0, -1.0, 1.0} ) );
    Univariate::SharedPointer p_p1( new UnivariatePolynomial<double>( {1.0, 1.0, 1.0} ) );

    using Basis            = ScalarFunction<1,double>;
    using Derivative       = Basis::derivative_type;
    using SecondDerivative = Derivative::derivative_type;

    auto p_f0 = Basis::SharedPointer(
        new SeparableScalarFunction<1,double>( {p_p0} )
    );

    auto p_f1 = Basis::SharedPointer(
        new SeparableScalarFunction<1,double>( {p_p1} )
    );

    Basis::SharedPointer p_sum;
    CIE_TEST_REQUIRE_NOTHROW( p_sum = makeSum( p_f0, p_f1 ) );

    CIE_TEST_CHECK( p_sum->evaluate( Basis::argument_type {-5.0} ) == Approx(52.0) );
    CIE_TEST_CHECK( p_sum->evaluate( Basis::argument_type {-2.0} ) == Approx(10.0) );
    CIE_TEST_CHECK( p_sum->evaluate( Basis::argument_type {-1.0} ) == Approx(4.0) );
    CIE_TEST_CHECK( p_sum->evaluate( Basis::argument_type {0.0} ) == Approx(2.0) );
    CIE_TEST_CHECK( p_sum->evaluate( Basis::argument_type {1.0} ) == Approx(4.0) );
    CIE_TEST_CHECK( p_sum->evaluate( Basis::argument_type {2.0} ) == Approx(10.0) );
    CIE_TEST_CHECK( p_sum->evaluate( Basis::argument_type {5.0} ) == Approx(52.0) );

    Derivative::SharedPointer p_derivative;
    CIE_TEST_REQUIRE_NOTHROW( p_derivative = p_sum->derivative() );
    CIE_TEST_CHECK( p_derivative->evaluate( Derivative::argument_type {-5.0} )[0] == Approx(-20.0) );
    CIE_TEST_CHECK( p_derivative->evaluate( Derivative::argument_type {-2.0} )[0] == Approx(-8.0) );
    CIE_TEST_CHECK( p_derivative->evaluate( Derivative::argument_type {-1.0} )[0] == Approx(-4.0) );
    CIE_TEST_CHECK( p_derivative->evaluate( Derivative::argument_type {0.0} )[0] == Approx(0.0) );
    CIE_TEST_CHECK( p_derivative->evaluate( Derivative::argument_type {1.0} )[0] == Approx(4.0) );
    CIE_TEST_CHECK( p_derivative->evaluate( Derivative::argument_type {2.0} )[0] == Approx(8.0) );
    CIE_TEST_CHECK( p_derivative->evaluate( Derivative::argument_type {5.0} )[0] == Approx(20.0) );

    SecondDerivative::SharedPointer p_secondDerivative;
    CIE_TEST_CHECK_NOTHROW( p_secondDerivative = p_derivative->derivative() );
    CIE_TEST_CHECK( p_secondDerivative->evaluate( SecondDerivative::argument_type {-5.0} )(0,0) == Approx(4.0) );
    CIE_TEST_CHECK( p_secondDerivative->evaluate( SecondDerivative::argument_type {-2.0} )(0,0) == Approx(4.0) );
    CIE_TEST_CHECK( p_secondDerivative->evaluate( SecondDerivative::argument_type {-1.0} )(0,0) == Approx(4.0) );
    CIE_TEST_CHECK( p_secondDerivative->evaluate( SecondDerivative::argument_type {0.0} )(0,0) == Approx(4.0) );
    CIE_TEST_CHECK( p_secondDerivative->evaluate( SecondDerivative::argument_type {1.0} )(0,0) == Approx(4.0) );
    CIE_TEST_CHECK( p_secondDerivative->evaluate( SecondDerivative::argument_type {2.0} )(0,0) == Approx(4.0) );
    CIE_TEST_CHECK( p_secondDerivative->evaluate( SecondDerivative::argument_type {5.0} )(0,0) == Approx(4.0) );
}


} // namespace cie::fem::maths