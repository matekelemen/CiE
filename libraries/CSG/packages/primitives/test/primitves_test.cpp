// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Linalg Includes ---
#include "linalg/packages/types/inc/vectortypes.hpp"
#include "linalg/packages/types/inc/arraytypes.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Primitive.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"

namespace cie::csg {

namespace boolean {


TEST_CASE( "Cube", "[primitives]" )
{
    CIE_TEST_CASE_INIT( "Cube" )

    const Size Dimension = 2;
    using CoordinateType = Double;
    using PrimitiveType  = Cube<Dimension,CoordinateType>;
    using PointType      = typename PrimitiveType::point_type;

    PointType base{ 0.0, 0.0 };
    CoordinateType length;

    // Unit cube
    {
        length = 1.0;
        REQUIRE_NOTHROW( PrimitiveType(base, length) );
        PrimitiveType primitive( base, length );
        CHECK( !primitive.isDegenerate() );
        REQUIRE( primitive.base().size() == Dimension );
        CHECK( primitive.base()[0] == Approx(base[0]) );
        CHECK( primitive.base()[1] == Approx(base[1]) );
        CHECK( primitive.length() == Approx(length) );
    }

    // Degenerate cube
    {
        length = 0.0;
        REQUIRE_NOTHROW( PrimitiveType(base, length) );
        PrimitiveType primitive( base, length );
        CHECK( primitive.isDegenerate() );
        REQUIRE( primitive.base().size() == Dimension );
        CHECK( primitive.base()[0] == Approx(base[0]) );
        CHECK( primitive.base()[1] == Approx(base[1]) );
        CHECK( primitive.length() == Approx(length) );
    }

    // Invalid cube
    {
        length = -1.0;
        CHECK_THROWS( PrimitiveType(base, length) );
    }
}



TEST_CASE( "Box", "[primitives]" )
{
    CIE_TEST_CASE_INIT( "Box" )

    const Size Dimension = 2;
    using CoordinateType = Double;
    using PrimitiveType  = Box<Dimension,CoordinateType>;
    using PointType      = typename PrimitiveType::point_type;

    PointType base{ 0.0, 0.0 };
    PointType lengths;

    // Unit cubeas box
    {
        lengths = { 1.0, 1.0 };
        REQUIRE_NOTHROW( PrimitiveType(base, lengths) );
        PrimitiveType primitive( base, lengths );
        CHECK( !primitive.isDegenerate() );
        REQUIRE( primitive.base().size() == Dimension );
        CHECK( primitive.base()[0] == Approx(base[0]) );
        CHECK( primitive.base()[1] == Approx(base[1]) );
        REQUIRE( primitive.lengths().size() == Dimension );
        CHECK( primitive.lengths()[0] == Approx(lengths[0]) );
        CHECK( primitive.lengths()[1] == Approx(lengths[1]) );
    }

    // Degenerate boxes
    {
        lengths = { 1.0, 0.0 };
        REQUIRE_NOTHROW( PrimitiveType(base, lengths) );
        PrimitiveType primitive( base, lengths );
        CHECK( primitive.isDegenerate() );

        lengths = { 0.0, 1.0 };
        REQUIRE_NOTHROW( PrimitiveType(base,lengths) );
        primitive = PrimitiveType( base, lengths );
        CHECK( primitive.isDegenerate() );
    }

    // Invalid boxes
    {
        lengths = { -1.0, 1.0 };
        CHECK_THROWS( PrimitiveType(base, lengths) );

        lengths = { 1.0, -1.0 };
        CHECK_THROWS( PrimitiveType(base, lengths) );
    }
}



TEST_CASE( "boolean::Cube", "[primitives]" )
{
    CIE_TEST_CASE_INIT( "boolean::Cube" )

    const Size dimension = 2;

    REQUIRE_NOTHROW( Cube<dimension,Double>(DoubleArray<dimension>({10.0,20.0}),2.0) );
    Cube<dimension,Double> cube( DoubleArray<dimension>({10.0,20.0}), 
                                    2.0 );
    CHECK( Cube<dimension,Double>::dimension == dimension );

    DoubleArray<dimension> point = {0.0, 0.0};

    REQUIRE_NOTHROW( cube.evaluate(point) );
    CHECK( cube.evaluate(point) == false );

    REQUIRE_NOTHROW( cube.evaluate(cube.base()) );
    CHECK( cube.evaluate( cube.base() ) == true );

    REQUIRE_NOTHROW( cube.evaluate( DoubleVector({11.999,21.999}) ) );
    CHECK( cube.evaluate( DoubleVector({11.999,21.999}) ) == true );
}


TEST_CASE( "boolean::Box", "[primitives]" )
{
    CIE_TEST_CASE_INIT( "boolean::Box" )

    const Size dimension = 2;
    Box<dimension,Double> box( DoubleArray<dimension>({10.0,20.0}), 
                               DoubleArray<dimension>({1.0,2.0}) );
    DoubleArray<dimension> point = {0.0, 0.0};

    REQUIRE_NOTHROW( box.evaluate(point) );
    CHECK( box.evaluate(point) == false );

    REQUIRE_NOTHROW( box.evaluate(box.base()) );
    CHECK( box.evaluate( box.base() ) == true );

    REQUIRE_NOTHROW( box.evaluate( DoubleVector({10.999999,21.99999}) ) );
    CHECK( box.evaluate( DoubleVector({10.999999,21.99999}) ) == true );
}


} // namespace boolean


} // namespace cie::csg