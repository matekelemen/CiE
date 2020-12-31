// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal includes ---
#include "CSG/packages/trees/inc/indexconverter.hpp"


namespace cie::csg {


CIE_TEST_CASE( "GridIndexConverter", "[trees]" )
{
    CIE_TEST_CASE_INIT( "GridIndexConverter" )

    const Size Dimension            = 2;
    Size numberOfPointsPerDimension = 3;

    CIE_TEST_REQUIRE_NOTHROW( GridIndexConverter<Dimension>(numberOfPointsPerDimension) );
    GridIndexConverter<Dimension> converter(numberOfPointsPerDimension);

    CIE_TEST_REQUIRE( converter.numberOfPoints() == 9 );
    
    UIntArray<Dimension> test;
    Size counter = 0;

    // 0
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 0 );
    CIE_TEST_CHECK( test[1] == 0 );

    // 1
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 1 );
    CIE_TEST_CHECK( test[1] == 0 );

    // 2
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 2 );
    CIE_TEST_CHECK( test[1] == 0 );

    // 3
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 0 );
    CIE_TEST_CHECK( test[1] == 1 );

    // 4
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 1 );
    CIE_TEST_CHECK( test[1] == 1 );

    // 5
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 2 );
    CIE_TEST_CHECK( test[1] == 1 );

    // 6
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 0 );
    CIE_TEST_CHECK( test[1] == 2 );

    // 7
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 1 );
    CIE_TEST_CHECK( test[1] == 2 );

    // 8
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 2 );
    CIE_TEST_CHECK( test[1] == 2 );

    #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    // 9 - error
    CIE_TEST_CHECK_THROWS( test = converter.convert(counter++) );
    #endif

    numberOfPointsPerDimension = 2;
    converter.setNumberOfPointsPerDimension( numberOfPointsPerDimension );
    CIE_TEST_REQUIRE( converter.numberOfPoints() == 4 );

    counter = 0;

    // 0
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 0 );
    CIE_TEST_CHECK( test[1] == 0 );

    // 1
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 1 );
    CIE_TEST_CHECK( test[1] == 0 );

    // 2
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 0 );
    CIE_TEST_CHECK( test[1] == 1 );

    // 3
    CIE_TEST_CHECK_NOTHROW( test = converter.convert(counter++) );
    CIE_TEST_REQUIRE( test.size() == Dimension );
    CIE_TEST_CHECK( test[0] == 1 );
    CIE_TEST_CHECK( test[1] == 1 );

    #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    // 4 - error
    CIE_TEST_CHECK_THROWS( test = converter.convert(counter++) );
    #endif
}


} // namespace cie::csg