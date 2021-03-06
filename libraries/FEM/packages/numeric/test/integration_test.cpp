// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "FEM/packages/numeric/inc/integration.hpp"


namespace cie::fem {


CIE_TEST_CASE( "AbsQuadrature", "[numeric]" )
{
    CIE_TEST_CASE_INIT( "AbsQuadrature" )

    const Size                              dimension = 2;
    typedef Double                          NT;
    typedef AbsQuadrature<dimension,NT>     Integrator;

    typename Integrator::point_container integrationPoints;
    integrationPoints.push_back( typename Integrator::point_type({-1.0,-1.0}) );
    integrationPoints.push_back( typename Integrator::point_type({1.0,-1.0}) );
    integrationPoints.push_back( typename Integrator::point_type({-1.0,1.0}) );
    integrationPoints.push_back( typename Integrator::point_type({1.0,1.0}) );

    typename Integrator::weight_container weights;
    weights.push_back(0.0);
    weights.push_back(1.0);
    weights.push_back(2.0);
    weights.push_back(3.0);

    // Test constructor
    CIE_TEST_REQUIRE_NOTHROW( Integrator(integrationPoints,weights) );
    Integrator integrator(integrationPoints,weights);

    // Test "integration"
    auto function = [](typename Integrator::point_type point) -> NT
    { return point[0] + point[1]; };

    CIE_TEST_REQUIRE_NOTHROW( integrator(function) );
    CIE_TEST_CHECK( integrator(function) == Approx(6.0) );

    // Test "integration" with precomputed values
    std::vector<NT> functionValues;
    for (const auto& point : integrationPoints)
        functionValues.push_back(function(point));

    CIE_TEST_REQUIRE_NOTHROW( integrator(functionValues.begin()) );
    CIE_TEST_CHECK( integrator(functionValues.begin()) == Approx(6.0) );
} // CIE_TEST_CASE AbsQuadrature




CIE_TEST_CASE( "Quadratures", "[numeric]" )
{
    CIE_TEST_CASE_INIT( "Quadratures" )

    // General init
    const Size                          Dimension = 2;
    typedef Double                      NT;
    typedef std::vector<NT>             Polynomial;

    // Helper functions
    auto evaluatePolynomial = []( const Polynomial& polynomial, NT coordinate ) -> NT
    {
        NT output   = 0.0;
        NT power    = 1.0;
        for (const auto& coefficient : polynomial)
        {
            output  += coefficient * power;
            power   *= coordinate;
        }
        return output;
    };

    auto integratePolynomial = [evaluatePolynomial]( const Polynomial& polynomial ) -> NT
    {
        Polynomial integratedPolynomial(polynomial.size() + 1);
        integratedPolynomial[0] = 0.0;

        for (Size i=0; i<polynomial.size(); ++i)
            integratedPolynomial[i+1] = 1.0/(NT(i)+1.0) * polynomial[i];

        return evaluatePolynomial(integratedPolynomial,1.0) - evaluatePolynomial(integratedPolynomial,-1.0);
    };

    // Test init
    const std::array<Polynomial,Dimension> testPolynomial = 
    {
        Polynomial({ 1.0, 2.0, 3.0, 4.0 }),
        Polynomial({ 5.0, 6.0, 7.0 })
    };
    NT check            = integratePolynomial(testPolynomial[0]) * integratePolynomial(testPolynomial[1]);
    auto testFunction   = [&evaluatePolynomial, &testPolynomial]( const typename AbsIntegrator<Dimension,NT>::point_type& point ) -> NT
    {
        NT output = 1.0;
        for (Size i=0; i<Dimension; ++i)
            output *= evaluatePolynomial( testPolynomial[i], point[i] );
        return output;
    };

    {
        CIE_TEST_CASE_INIT( "GaussLegendreQuadrature" )
        typedef GaussLegendreQuadrature<Dimension, NT> Integrator;

        for (Size integrationOrder=3; integrationOrder<6; ++integrationOrder)
        {
            CIE_TEST_REQUIRE_NOTHROW(Integrator(integrationOrder));
            Integrator integrator(integrationOrder);
            CIE_TEST_REQUIRE_NOTHROW( integrator(testFunction) );
            CIE_TEST_CHECK( integrator(testFunction) == Approx(check) );
        }
    }

    {
        CIE_TEST_CASE_INIT( "GaussLobattoQuadrature" )
        typedef GaussLobattoQuadrature<Dimension, NT> Integrator;

        for (Size integrationOrder=3; integrationOrder<6; ++integrationOrder)
        {
            CIE_TEST_REQUIRE_NOTHROW(Integrator(integrationOrder));
            Integrator integrator(integrationOrder);
            CIE_TEST_REQUIRE_NOTHROW( integrator(testFunction) );
            CIE_TEST_CHECK( integrator(testFunction) == Approx(check) );
        }
    }
} // CIE_TEST_CASE Quadratures


} // namespace cie::fem