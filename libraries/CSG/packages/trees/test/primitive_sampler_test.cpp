// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Inludes ---
#include "CSG/packages/trees/inc/primitive_sampler.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::csg {


CIE_TEST_CASE( "CartesianGridSampler", "[trees]" )
{
    CIE_TEST_CASE_INIT( "CartesianGridSampler" )

    // Dimension and coordinate type
    const Size      Dimension = 2;
    typedef Double  CT;

    {
        CIE_TEST_CASE_INIT( "Cube sampler" )

        // Primitive and sampler types
        typedef boolean::Cube<Dimension,CT>         PrimitiveType;
        typedef CartesianGridSampler<PrimitiveType> Sampler;

        // Primitive properties
        const typename PrimitiveType::point_type        base = {1.0, 2.0};
        const typename PrimitiveType::coordinate_type   length = 2.0;

        PrimitiveType primitive( base, length );

        // Check construction
        CIE_TEST_REQUIRE_NOTHROW( std::make_shared<Sampler>(1) );
        Sampler sampler1(1);

        CIE_TEST_REQUIRE_NOTHROW( std::make_shared<Sampler>(2) );
        Sampler sampler2(2);

        typename PrimitiveType::point_type point;

        // Check sizes
        CIE_TEST_CHECK_NOTHROW( sampler1.size() );
        CIE_TEST_REQUIRE( sampler1.size() == 1 );
        CIE_TEST_CHECK_NOTHROW( sampler2.size() );
        CIE_TEST_REQUIRE( sampler2.size() == 4 );

        // Check center sampling
        CIE_TEST_REQUIRE_NOTHROW( sampler1.getSamplePoint(primitive,0) );
        point = sampler1.getSamplePoint(primitive,0); 
        CIE_TEST_CHECK( point[0] == 2.0 );
        CIE_TEST_CHECK( point[1] == 3.0 );

        // Check sampling
        CIE_TEST_REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,0) );
        point = sampler2.getSamplePoint(primitive,0); 
        CIE_TEST_CHECK( point[0] == 1.0 );
        CIE_TEST_CHECK( point[1] == 2.0 );

        CIE_TEST_REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,1) );
        point = sampler2.getSamplePoint(primitive,1); 
        CIE_TEST_CHECK( point[0] == 3.0 );
        CIE_TEST_CHECK( point[1] == 2.0 );

        CIE_TEST_REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,2) );
        point = sampler2.getSamplePoint(primitive,2); 
        CIE_TEST_CHECK( point[0] == 1.0 );
        CIE_TEST_CHECK( point[1] == 4.0 );

        CIE_TEST_REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,3) );
        point = sampler2.getSamplePoint(primitive,3); 
        CIE_TEST_CHECK( point[0] == 3.0 );
        CIE_TEST_CHECK( point[1] == 4.0 );
    }

    {
        CIE_TEST_CASE_INIT( "Box sampler" )

        // Primitive and sampler types
        typedef boolean::Box<Dimension,CT>          PrimitiveType;
        typedef CartesianGridSampler<PrimitiveType> Sampler;

        // Primitive properties
        const typename PrimitiveType::point_type        base = {1.0, 2.0};
        const typename PrimitiveType::point_type        lengths = {2.0,4.0};

        PrimitiveType primitive( base, lengths );

        // Check construction
        CIE_TEST_REQUIRE_NOTHROW( std::make_shared<Sampler>(1) );
        Sampler sampler1(1);

        CIE_TEST_REQUIRE_NOTHROW( std::make_shared<Sampler>(1) );
        Sampler sampler2(2);

        typename PrimitiveType::point_type point;

        // Check center sampling
        CIE_TEST_REQUIRE_NOTHROW( sampler1.getSamplePoint(primitive,0) );
        point = sampler1.getSamplePoint(primitive,0); 
        CIE_TEST_CHECK( point[0] == 2.0 );
        CIE_TEST_CHECK( point[1] == 4.0 );

        // Check sampling
        CIE_TEST_REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,0) );
        point = sampler2.getSamplePoint(primitive,0); 
        CIE_TEST_CHECK( point[0] == 1.0 );
        CIE_TEST_CHECK( point[1] == 2.0 );

        CIE_TEST_REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,1) );
        point = sampler2.getSamplePoint(primitive,1); 
        CIE_TEST_CHECK( point[0] == 3.0 );
        CIE_TEST_CHECK( point[1] == 2.0 );

        CIE_TEST_REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,2) );
        point = sampler2.getSamplePoint(primitive,2); 
        CIE_TEST_CHECK( point[0] == 1.0 );
        CIE_TEST_CHECK( point[1] == 6.0 );

        CIE_TEST_REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,3) );
        point = sampler2.getSamplePoint(primitive,3); 
        CIE_TEST_CHECK( point[0] == 3.0 );
        CIE_TEST_CHECK( point[1] == 6.0 );
    }
}


} // namespace cie::csg