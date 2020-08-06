// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/integration.hpp"


namespace cie::fem {


TEST_CASE( "AbsQuadrature", "[numeric]" )
{
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
    REQUIRE_NOTHROW( Integrator(integrationPoints,weights) );
    Integrator integrator(integrationPoints,weights);

    // Test "integration"
    auto function = [](typename Integrator::point_type point) -> NT
    { return point[0] + point[1]; };

    REQUIRE_NOTHROW( integrator(function) );
    CHECK( integrator(function) == Approx(6.0) );
}


} // namespace cie::fem