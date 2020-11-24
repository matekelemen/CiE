// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Inludes ---
#include "CSG/packages/trees/inc/primitive_sampler.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::csg {


TEST_CASE( "CubeSampler", "[trees]" )
{
    CIE_TEST_CASE_INIT( "CubeSampler" )

    // Dimension and coordinate type
    const Size      Dimension = 2;
    typedef Double  CT;

    // Primitive and sampler types
    typedef boolean::Cube<Dimension,CT> PrimitiveType;
    typedef CubeSampler<Dimension,CT>   Sampler;

    // Primitive properties
    const typename PrimitiveType::point_type        base = {1.0, 2.0};
    const typename PrimitiveType::coordinate_type   length = 2.0;

    PrimitiveType primitive( base, length );

    // Check construction
    REQUIRE_NOTHROW( std::make_shared<Sampler>(1) );
    Sampler sampler1(1);

    REQUIRE_NOTHROW( std::make_shared<Sampler>(2) );
    Sampler sampler2(2);

    typename PrimitiveType::point_type point;

    // Check sizes
    CHECK_NOTHROW( sampler1.size() );
    REQUIRE( sampler1.size() == 1 );
    CHECK_NOTHROW( sampler2.size() );
    REQUIRE( sampler2.size() == 4 );

    // Check center sampling
    REQUIRE_NOTHROW( sampler1.getSamplePoint(primitive,0) );
    point = sampler1.getSamplePoint(primitive,0); 
    CHECK( point[0] == 2.0 );
    CHECK( point[1] == 3.0 );

    // Check sampling
    REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,0) );
    point = sampler2.getSamplePoint(primitive,0); 
    CHECK( point[0] == 1.0 );
    CHECK( point[1] == 2.0 );

    REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,1) );
    point = sampler2.getSamplePoint(primitive,1); 
    CHECK( point[0] == 3.0 );
    CHECK( point[1] == 2.0 );

    REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,2) );
    point = sampler2.getSamplePoint(primitive,2); 
    CHECK( point[0] == 1.0 );
    CHECK( point[1] == 4.0 );

    REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,3) );
    point = sampler2.getSamplePoint(primitive,3); 
    CHECK( point[0] == 3.0 );
    CHECK( point[1] == 4.0 );
}



TEST_CASE( "BoxSampler", "[trees]" )
{
    CIE_TEST_CASE_INIT( "BoxSampler" )

    // Dimension and coordinate type
    typedef Double  CT;
    const Size      Dimension = 2;

    // Primitive and sampler types
    typedef boolean::Box<Dimension,CT> PrimitiveType;
    typedef BoxSampler<Dimension,CT>   Sampler;

    // Primitive properties
    const typename PrimitiveType::point_type        base = {1.0, 2.0};
    const typename PrimitiveType::point_type        lengths = {2.0,4.0};

    PrimitiveType primitive( base, lengths );

    // Check construction
    REQUIRE_NOTHROW( std::make_shared<Sampler>(1) );
    Sampler sampler1(1);

    REQUIRE_NOTHROW( std::make_shared<Sampler>(1) );
    Sampler sampler2(2);

    typename PrimitiveType::point_type point;

    // Check center sampling
    REQUIRE_NOTHROW( sampler1.getSamplePoint(primitive,0) );
    point = sampler1.getSamplePoint(primitive,0); 
    CHECK( point[0] == 2.0 );
    CHECK( point[1] == 4.0 );

    // Check sampling
    REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,0) );
    point = sampler2.getSamplePoint(primitive,0); 
    CHECK( point[0] == 1.0 );
    CHECK( point[1] == 2.0 );

    REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,1) );
    point = sampler2.getSamplePoint(primitive,1); 
    CHECK( point[0] == 3.0 );
    CHECK( point[1] == 2.0 );

    REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,2) );
    point = sampler2.getSamplePoint(primitive,2); 
    CHECK( point[0] == 1.0 );
    CHECK( point[1] == 6.0 );

    REQUIRE_NOTHROW( sampler2.getSamplePoint(primitive,3) );
    point = sampler2.getSamplePoint(primitive,3); 
    CHECK( point[0] == 3.0 );
    CHECK( point[1] == 6.0 );
}


} // namespace cie::csg