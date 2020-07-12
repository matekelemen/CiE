// --- External Includes ---
#include "catch.hpp"

// --- Internal Inludes ---
#include "../inc/primitive_sampler.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::csg {


TEST_CASE( "CubeSampler", "[trees]" )
{
    // Dimension and coordinate type
    typedef Double  CT;
    const Size      Dimension = 2;

    // Primitive and sampler types
    typedef boolean::CSGCube<Dimension,CT>          PrimitiveType;
    typedef CubeSampler<Dimension,1,CT>             Sampler1;
    typedef CubeSampler<Dimension,2,CT>             Sampler2;

    // Primitive properties
    const typename PrimitiveType::point_type        base = {1.0, 2.0};
    const typename PrimitiveType::coordinate_type   length = 2.0;

    PrimitiveType primitive( base, length );

    // Check construction
    REQUIRE_NOTHROW( std::make_shared<Sampler1>() );
    REQUIRE_NOTHROW( std::make_shared<Sampler2>() );
    Sampler1 sampler1;
    Sampler2 sampler2;
    typename PrimitiveType::point_type point;

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
    // Dimension and coordinate type
    typedef Double  CT;
    const Size      Dimension = 2;

    // Primitive and sampler types
    typedef boolean::CSGBox<Dimension,CT>           PrimitiveType;
    typedef BoxSampler<Dimension,1,CT>              Sampler1;
    typedef BoxSampler<Dimension,2,CT>              Sampler2;

    // Primitive properties
    const typename PrimitiveType::point_type        base = {1.0, 2.0};
    const typename PrimitiveType::point_type        lengths = {2.0,4.0};

    PrimitiveType primitive( base, lengths );

    // Check construction
    REQUIRE_NOTHROW( std::make_shared<Sampler1>() );
    REQUIRE_NOTHROW( std::make_shared<Sampler2>() );
    Sampler1 sampler1;
    Sampler2 sampler2;
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