// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/CornerSampler.hpp"


namespace cie::csg {


CIE_TEST_CASE( "CornerSampler", "[trees]" )
{
    CIE_TEST_CASE_INIT( "CornerSampler" )

    {
        CIE_TEST_CASE_INIT( "Cube" )

        using Primitive = Cube<2,double>;
        using Sampler = CornerSampler<Primitive>;

        Primitive primitive( {-1.0, 0.0}, 2.0 );

        CIE_TEST_CHECK_NOTHROW( Sampler() );
        Sampler sampler;
        CIE_TEST_REQUIRE( sampler.size() == 4 );

        Primitive::point_type samplePoint;
        CIE_TEST_CHECK_NOTHROW( samplePoint = sampler.getSamplePoint(primitive, 0) );
        CIE_TEST_CHECK( samplePoint[0] == Approx(-1.0) );
        CIE_TEST_CHECK( samplePoint[1] == Approx(0.0) );

        CIE_TEST_CHECK_NOTHROW( samplePoint = sampler.getSamplePoint(primitive, 1) );
        CIE_TEST_CHECK( samplePoint[0] == Approx(1.0) );
        CIE_TEST_CHECK( samplePoint[1] == Approx(0.0) );

        CIE_TEST_CHECK_NOTHROW( samplePoint = sampler.getSamplePoint(primitive, 2) );
        CIE_TEST_CHECK( samplePoint[0] == Approx(-1.0) );
        CIE_TEST_CHECK( samplePoint[1] == Approx(2.0) );

        CIE_TEST_CHECK_NOTHROW( samplePoint = sampler.getSamplePoint(primitive, 3) );
        CIE_TEST_CHECK( samplePoint[0] == Approx(1.0) );
        CIE_TEST_CHECK( samplePoint[1] == Approx(2.0) );
    }

    {
        CIE_TEST_CASE_INIT( "Box" )

        using Primitive = Box<2,double>;
        using Sampler = CornerSampler<Primitive>;

        Primitive primitive( Primitive::point_type {-1.0, 0.0}, Primitive::point_type {1.0, 2.0} );

        CIE_TEST_CHECK_NOTHROW( Sampler() );
        Sampler sampler;
        CIE_TEST_REQUIRE( sampler.size() == 4 );

        Primitive::point_type samplePoint;
        CIE_TEST_CHECK_NOTHROW( samplePoint = sampler.getSamplePoint(primitive, 0) );
        CIE_TEST_CHECK( samplePoint[0] == Approx(-1.0) );
        CIE_TEST_CHECK( samplePoint[1] == Approx(0.0) );

        CIE_TEST_CHECK_NOTHROW( samplePoint = sampler.getSamplePoint(primitive, 1) );
        CIE_TEST_CHECK( samplePoint[0] == Approx(0.0) );
        CIE_TEST_CHECK( samplePoint[1] == Approx(0.0) );

        CIE_TEST_CHECK_NOTHROW( samplePoint = sampler.getSamplePoint(primitive, 2) );
        CIE_TEST_CHECK( samplePoint[0] == Approx(-1.0) );
        CIE_TEST_CHECK( samplePoint[1] == Approx(2.0) );

        CIE_TEST_CHECK_NOTHROW( samplePoint = sampler.getSamplePoint(primitive, 3) );
        CIE_TEST_CHECK( samplePoint[0] == Approx(0.0) );
        CIE_TEST_CHECK( samplePoint[1] == Approx(2.0) );
    }
}


} // namespace cie::csg