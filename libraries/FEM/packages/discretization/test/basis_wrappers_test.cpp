// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "FEM/packages/discretization/inc/basis_wrappers.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::fem {


CIE_TEST_CASE( "basis_wrappers", "[discretization]" )
{
    CIE_TEST_CASE_INIT( "Basis wrappers" )

    const Size Dimension        = 2;
    using NT                    = Double;
    using Container             = std::vector<NT>;
    using Basis                 = detail::TensorProductBasis<Container,Dimension>;
    using BasisDerivatives      = detail::TensorProductDerivatives<Container,Dimension>;

    typename Basis::container_array basisValues;
    utils::resize(basisValues,Dimension);
    basisValues[0] = Container({ 1.0, 2.0 });
    basisValues[1] = Container({ 3.0, 4.0 });

    typename BasisDerivatives::container_array derivativeValues;
    utils::resize(derivativeValues,Dimension);
    derivativeValues[0] = Container({ 5.0, 6.0 });
    derivativeValues[1] = Container({ 7.0, 8.0 });

    CIE_TEST_REQUIRE_NOTHROW( Basis(basisValues) );
    Basis basis(basisValues);

    CIE_TEST_REQUIRE_NOTHROW( BasisDerivatives(basisValues,derivativeValues) );
    BasisDerivatives basisDerivative(basisValues,derivativeValues);

    {
        CIE_TEST_CASE_INIT( "TensorProductBasis" )
        for (Size j=0; j<Dimension; ++j)
            for (Size i=0; i<Dimension; ++i)
            {
                CIE_TEST_REQUIRE_NOTHROW( basis.product() );
                CIE_TEST_CHECK( basis.product() == Approx(NT( (i+1)*(j+3) )) );
                CIE_TEST_REQUIRE_NOTHROW( ++basis );
            }
        CIE_TEST_REQUIRE_NOTHROW( basis.reset() );
    }

    {
        CIE_TEST_CASE_INIT( "TensorProductDerivatives" )
        typename BasisDerivatives::point_type derivative;

        CIE_TEST_REQUIRE_NOTHROW( derivative = basisDerivative.product() );
        CIE_TEST_REQUIRE( derivative.size() == Dimension );
        CIE_TEST_CHECK( derivative[0] == Approx(15.0) );
        CIE_TEST_CHECK( derivative[1] == Approx(7.0) );
        CIE_TEST_REQUIRE_NOTHROW( ++basisDerivative );

        CIE_TEST_REQUIRE_NOTHROW( derivative = basisDerivative.product() );
        CIE_TEST_REQUIRE( derivative.size() == Dimension );
        CIE_TEST_CHECK( derivative[0] == Approx(18.0) );
        CIE_TEST_CHECK( derivative[1] == Approx(14.0) );
        CIE_TEST_REQUIRE_NOTHROW( ++basisDerivative );

        CIE_TEST_REQUIRE_NOTHROW( derivative = basisDerivative.product() );
        CIE_TEST_REQUIRE( derivative.size() == Dimension );
        CIE_TEST_CHECK( derivative[0] == Approx(20.0) );
        CIE_TEST_CHECK( derivative[1] == Approx(8.0) );
        CIE_TEST_REQUIRE_NOTHROW( ++basisDerivative );

        CIE_TEST_REQUIRE_NOTHROW( derivative = basisDerivative.product() );
        CIE_TEST_REQUIRE( derivative.size() == Dimension );
        CIE_TEST_CHECK( derivative[0] == Approx(24.0) );
        CIE_TEST_CHECK( derivative[1] == Approx(16.0) );
        CIE_TEST_REQUIRE_NOTHROW( ++basisDerivative );
    }
    
} // CIE_TEST_CASE TensorProductBasis


} // namespace cie::fem