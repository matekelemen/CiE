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

    Univariate::SharedPointer p_p0( new UnivariatePolynomial<double>( {1.0, -1.0} ) );
    Univariate::SharedPointer p_p1( new UnivariatePolynomial<double>( {1.0, 1.0} ) );

    using Basis = ScalarFunction<1,double>;

    auto p_f0 = Basis::SharedPointer(
        new SeparableScalarFunction<1,double>( {p_p0} )
    );

    auto p_f1 = Basis::SharedPointer(
        new SeparableScalarFunction<1,double>( {p_p1} )
    );

    // Create function (1-x)*(1+x)
    Basis::SharedPointer p_product;
    CIE_TEST_REQUIRE_NOTHROW( p_product = makeProduct( p_f0, p_f1 ) );

    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {-5.0} ) == Approx(-24.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {-2.0} ) == Approx(-3.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {-1.0} ) == Approx(0.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {0.0} ) == Approx(1.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {1.0} ) == Approx(0.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {2.0} ) == Approx(-3.0) );
    CIE_TEST_CHECK( p_product->evaluate( Basis::argument_type {5.0} ) == Approx(-24.0) );
}


CIE_TEST_CASE( "makeProduct: scalar * vector", "[maths]" )
{
    CIE_TEST_CASE_INIT( "makeProduct: scalar * vector" )

    using Univariate    = UnivariateScalarFunction<int>;
    using ScalarOperand = ScalarFunction<3, int>;
    using VectorOperand = VectorFunction<2, 3, int>;

    Univariate::SharedPointer p_p0( new UnivariatePolynomial<int>( {1, -1} ) );
    Univariate::SharedPointer p_p1( new UnivariatePolynomial<int>( {1, 1} ) );
    Univariate::SharedPointer p_p2( new UnivariatePolynomial<int>( {-5, -5} ) );

    auto p_scalarOperand = ScalarOperand::SharedPointer(
        new SeparableScalarFunction<3, int>( {p_p0, p_p1, p_p2} )
    );

    auto p_vectorComponent0 = SeparableScalarFunction<3, int>::SharedPointer(
        new SeparableScalarFunction<3, int>( {p_p0, p_p1, p_p2} )
    );

    auto p_vectorComponent1 = SeparableScalarFunction<3, int>::SharedPointer(
        new SeparableScalarFunction<3, int>( {p_p2, p_p1, p_p0} )
    );

    auto p_vectorOperand = VectorOperand::SharedPointer(
        new SeparableVectorFunction<2, 3, int>({
            p_vectorComponent0,
            p_vectorComponent1
        })
    );
}


} // namespace cie::fem::maths