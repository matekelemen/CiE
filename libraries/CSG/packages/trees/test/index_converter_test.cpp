// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal includes ---
#include "CSG/packages/trees/inc/indexconverter.hpp"


namespace cie::csg {


TEST_CASE( "GridIndexConverter", "[trees]" )
{
    CIE_TEST_CASE_INIT( "GridIndexConverter" )

    const Size Dimension            = 2;
    Size numberOfPointsPerDimension = 3;

    REQUIRE_NOTHROW( GridIndexConverter<Dimension>(numberOfPointsPerDimension) );
    GridIndexConverter<Dimension> converter(numberOfPointsPerDimension);

    REQUIRE( converter.numberOfPoints() == 9 );
    
    UIntArray<Dimension> test;
    Size counter = 0;

    // 0
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 0 );
    CHECK( test[1] == 0 );

    // 1
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 1 );
    CHECK( test[1] == 0 );

    // 2
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 2 );
    CHECK( test[1] == 0 );

    // 3
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 0 );
    CHECK( test[1] == 1 );

    // 4
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 1 );
    CHECK( test[1] == 1 );

    // 5
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 2 );
    CHECK( test[1] == 1 );

    // 6
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 0 );
    CHECK( test[1] == 2 );

    // 7
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 1 );
    CHECK( test[1] == 2 );

    // 8
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 2 );
    CHECK( test[1] == 2 );

    #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    // 9 - error
    CHECK_THROWS( test = converter.convert(counter++) );
    #endif

    numberOfPointsPerDimension = 2;
    converter.setNumberOfPointsPerDimension( numberOfPointsPerDimension );
    REQUIRE( converter.numberOfPoints() == 4 );

    counter = 0;

    // 0
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 0 );
    CHECK( test[1] == 0 );

    // 1
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 1 );
    CHECK( test[1] == 0 );

    // 2
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 0 );
    CHECK( test[1] == 1 );

    // 3
    CHECK_NOTHROW( test = converter.convert(counter++) );
    REQUIRE( test.size() == Dimension );
    CHECK( test[0] == 1 );
    CHECK( test[1] == 1 );

    #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    // 4 - error
    CHECK_THROWS( test = converter.convert(counter++) );
    #endif
}


} // namespace cie::csg