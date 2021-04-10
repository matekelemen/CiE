// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/operation/inc/Product.hpp"
#include "FEM/packages/maths/inc/SeparableScalarFunction.hpp"
#include "FEM/packages/maths/packages/polynomial/inc/UnivariatePolynomial.hpp"


namespace cie::fem::maths {


CIE_TEST_CASE( "makeProduct: scalar * scalar", "[maths]" )
{
    CIE_TEST_CASE_INIT( "makeProduct: scalar * scalar" )

    using Univariate = UnivariateScalarFunction<double>;
    using UnivariatePtr = std::shared_ptr<Univariate>;

    UnivariatePtr p_p0( new UnivariatePolynomial<double>( {1.0, -1.0} ) );
    UnivariatePtr p_p1( new UnivariatePolynomial<double>( {1.0, 1.0} ) );

    using Basis = ScalarFunction<1,double>;
    using BasisPtr = std::shared_ptr<Basis>;

    auto p_f0 = BasisPtr(
        new SeparableScalarFunction<1,double>( {p_p0} )
    );

    auto p_f1 = BasisPtr(
        new SeparableScalarFunction<1,double>( {p_p1} )
    );

    // Create function (1-x)*(1+x)
    BasisPtr p_product;
    CIE_TEST_REQUIRE_NOTHROW( p_product = makeProduct( p_f0, p_f1 ) );

    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {-5.0} ) == Approx(-24.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {-2.0} ) == Approx(-3.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {-1.0} ) == Approx(0.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {0.0} ) == Approx(1.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {1.0} ) == Approx(0.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {2.0} ) == Approx(-3.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {5.0} ) == Approx(-24.0) );
}


} // namespace cie::fem::maths