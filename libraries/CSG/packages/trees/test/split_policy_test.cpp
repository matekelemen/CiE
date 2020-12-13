// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/split_policy.hpp"

// --- STL Includes ---
#include <array>
#include <vector>
#include <deque>


namespace cie::csg {


TEST_CASE( "MidPointSplitPolicy", "[trees]" )
{
    CIE_TEST_CASE_INIT( "MidPointSplitPolicy" )

    const Size Dimension    = 2;
    using CoordinateType    = Double;
    using PointType         = std::array<CoordinateType,Dimension>;
    using ValueType         = Size;

    std::vector<PointType> points;
    points.push_back( PointType({ 0.0, 0.0 }) );
    points.push_back( PointType({ 1.0, 0.0 }) );
    points.push_back( PointType({ 0.0, 1.0 }) );
    points.push_back( PointType({ 1.0, 1.0 }) );

    std::deque<ValueType> values { 0, 1, 2, 3 };

    using Splitter = MidPointSplitPolicy< typename std::vector<PointType>::iterator,
                                          typename std::deque<ValueType>::iterator >;
    Splitter splitter;
    PointType splitPoint = splitter( values.begin(),
                                     values.end(),
                                     points.begin() );

    CHECK( splitPoint.size() == Dimension );
    for ( const auto& r_component : splitPoint )
        CHECK( r_component == Approx(0.5) );
}



TEST_CASE( "LinearSplitPolicy", "[trees]" )
{
    CIE_TEST_CASE_INIT( "LinearSplitPolicy" )

    const Size Dimension    = 2;
    using CoordinateType    = Double;
    using PointType         = std::array<CoordinateType,Dimension>;
    using ValueType         = int;

    std::vector<PointType> points;
    points.push_back( PointType({ 0.0, 0.0 }) );
    points.push_back( PointType({ 1.0, 0.0 }) );
    points.push_back( PointType({ 0.0, 1.0 }) );
    points.push_back( PointType({ 1.0, 1.0 }) );

    std::deque<ValueType> values { -1, 2, 3, -3 };

    using Splitter = LinearSplitPolicy< typename std::vector<PointType>::iterator,
                                        typename std::deque<ValueType>::iterator >;
    Splitter splitter;
    PointType splitPoint = splitter( values.begin(),
                                     values.end(),
                                     points.begin() );

    REQUIRE( splitPoint.size() == Dimension );
    CHECK( splitPoint[0] == Approx( 2.0/3.0 ) );
    CHECK( splitPoint[1] == Approx( 1.0 ) );
}


} // namespace cie::csg