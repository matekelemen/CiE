#include "meshgenerator.hpp"
#include "meshgenerator_helper.hpp"
#include "catch.hpp"

#include <vector>
#include <cmath>
#include <numeric>

namespace cie {
namespace meshkernel
{

const double PI = 3.141592653589793;

// Test class to avoid code reuse
struct RegionWithKinkFixture
{
  /* Create the following polygon:
   *   6    4    7
   *     o     o
   *     | \ / |
   *   3 o  o  o 5
   *     |     |
   *     o--o--o
   *   0    1    2
   */

  Vertex2DVector vertices;
  IndexVector region;

  RegionWithKinkFixture( ) :
    vertices{ { 0.0, 0.0 }, { 0.5, 0.0 }, { 1.0, 0.0 }, { 0.0, 0.5 },
              { 0.5, 0.5 }, { 1.0, 0.5 }, { 0.0, 1.0 }, { 1.0, 1.0 } },
    region{ 0, 1, 2, 5, 7, 4, 6, 3 }
  { }
};

// Test case that uses the struct defined above. By some inheritance magic of catch we can use vertices and region here.
TEST_CASE_METHOD( RegionWithKinkFixture, "computeAngle" )
{
  std::array<double, 8> anglesCounterClockwise;
  std::array<double, 8> anglesClockwise;

  auto reversedRegion = region;
  std::reverse( reversedRegion.begin( ), reversedRegion.end( ) );

  // Compute angles first
  REQUIRE_NOTHROW( anglesCounterClockwise[0] = meshgeneratorhelper::computeAngle( vertices, region, 7, 0, 1 ) );
  REQUIRE_NOTHROW( anglesCounterClockwise[7] = meshgeneratorhelper::computeAngle( vertices, region, 6, 7, 0 ) );
  REQUIRE_NOTHROW( anglesClockwise[0] = meshgeneratorhelper::computeAngle( vertices, reversedRegion, 7, 0, 1 ) );
  REQUIRE_NOTHROW( anglesClockwise[7] = meshgeneratorhelper::computeAngle( vertices, reversedRegion, 6, 7, 0 ) );

  for( size_t i = 1; i < 7; ++i )
  {
    REQUIRE_NOTHROW( anglesCounterClockwise[i] = meshgeneratorhelper::computeAngle( vertices, region, i - 1, i, i + 1 ) );
    REQUIRE_NOTHROW( anglesClockwise[i] = meshgeneratorhelper::computeAngle( vertices, reversedRegion, i - 1, i, i + 1 ) );
  }

  // Those are the values we expect
  std::array<double, 8> expectedAngles{ PI / 2.0, PI, PI / 2.0, PI, PI / 4.0, PI / 2.0, PI / 4.0, PI };

  // Compare expected and computed values
  for( size_t i = 0; i < 8; ++i )
  {
    CHECK( anglesCounterClockwise[i] == Approx( expectedAngles[i] ) );
    CHECK( anglesClockwise[7 - i] == Approx( expectedAngles[i] ) );
  } // for i

} // computeAngle

// Attempts to check if zero angle is stable (not giving nans).
TEST_CASE( "computeAngleZero" )
{
  Vertex2DVector vertices{ {  1.0 / 3.0,  5.0 / 7.0 },
                           {  2.0 / 3.0, 11.0 / 7.0 },
                           { -1.0 / 3.0, -7.0 / 7.0 } };

  IndexVector region{ 0, 1, 2 };

  double angle;

  REQUIRE_NOTHROW( angle = meshgeneratorhelper::computeAngle( vertices, region, 0, 1, 2 ) );

  CHECK( angle == Approx( 0.0 ).margin( 1e-7 ) );

} // computeAngle

// Checks if zero length is caught.
TEST_CASE( "computeAngleZeroLength" )
{
  Vertex2DVector vertices{ { 2.0, 0.0 },
                           { 2.0, 0.0 },
                           { 1.0, 0.0 } };
  IndexVector region{ 0, 1, 2 };

  double angle;

  REQUIRE_NOTHROW( angle = meshgeneratorhelper::computeAngle( vertices, region, 0, 1, 2 ) );

  CHECK( angle == Approx( 0.0 ).margin( 1e-7 ) );

} // computeAngle

TEST_CASE_METHOD( RegionWithKinkFixture, "computeSmallestAngle" )
{
  double pi = PI;
  double pi_2 = pi / 2.0;
  double pi_4 = pi / 4.0;
  double atanP5 = std::atan( 0.5 );

  // These are the expected angles for different division lines
  std::array<std::vector<double>, 6> minAngles { {
    { 0.0 , atanP5       , 0.0 , pi_4         , 0.0, 0.0 }, // 0 - 2, 0 - 3, 0 - 4, 0 - 5, 0 - 6, 0 - 7
    { pi_4, pi_4 - atanP5, pi_2, pi_4 - atanP5, pi_4     }, // 1 - 3, 1 - 4, 1 - 5, 1 - 6, 1 - 7
    { 0.0 , pi_4         , 0.0 , atanP5                  }, // 2 - 4, 2 - 5, 2 - 6, 2 - 7
    { pi_4, pi_4 - atanP5, pi_2                          }, // 3 - 5, 3 - 6, 3 - 7
    { pi_4, pi_4 - atanP5                                }, // 4 - 6, 4 - 7
    { pi_4                                               }  // 5 - 7
  } };

  // Compute angles and check if they match the expected ones
  for( size_t i = 0; i < 6; ++i )
  {
    for( size_t j = i + 2; j < 8; ++j )
    {
      double angle = -1.0;

      REQUIRE_NOTHROW( angle = meshgeneratorhelper::computeSmallestAngle( vertices, region, i, j ) );

      CHECK( angle == Approx( minAngles[i][j - i - 2] ).margin( 1e-7 ) );
    } // for i
  } // for j

} // computeSmallestAngle

struct CircularRegionFixture
{
  /* Create the following polygon:
   *     6        7
   *       o----o
   *     /       \
   * 4  o         o  5
   *    |         |
   * 2  o         o  3
   *     \       /
   *       o----o
   *     0        1
   */

  Vertex2DVector vertices;
  std::vector<IndexVector> regions;

  CircularRegionFixture( ) :
    vertices{ { 1.0, 0.0 }, { 2.0, 0.0 }, { 0.0, 1.0 }, { 3.0, 1.0 },
              { 0.0, 2.0 }, { 3.0, 2.0 }, { 1.0, 3.0 }, { 2.0, 3.0 } },
    regions{{ 0, 1, 3, 5, 7, 6, 4, 2 }}
  { }
};

// Helper function for next test case
double polygonArea( const Vertex2DVector& vertices, const IndexVector& indices )
{
  double area = vertices[indices.back( )][0] * vertices[indices[0]][1] -
                vertices[indices[0]][0] * vertices[indices.back( )][1];

  for( size_t i = 0; i < indices.size( ) - 1; ++i )
  {
    size_t id1 = indices[i];
    size_t id2 = indices[i + 1];

    area += vertices[id1][0] * vertices[id2][1] - vertices[id2][0] * vertices[id1][1];
  }

  return std::abs( 0.5 * area ); // minus if clockwise
}

TEST_CASE_METHOD( CircularRegionFixture, "divideRegion" )
{
  REQUIRE( polygonArea( vertices, regions[0] ) == Approx( 7.0 ) );

  std::array<size_t, 5> numberOfNewSegments{ 2, 3, 3, 3, 2 };

  for( size_t i = 0; i < 4; ++i )
  {
    for( size_t j = i + 2; j < 8; ++j )
    {
      // Skip invalid division
      if( i == 0 && j == 7 )
      {
        continue;
      }

      auto v = vertices;
      auto r = regions;

      // The dividing line has tree segments, except when it divides to a vertex with distance two
      size_t n = numberOfNewSegments[j - i - 2];

      REQUIRE_NOTHROW( meshgeneratorhelper::divideRegion( v, r, 0, i, j, 1.0 ) );
      REQUIRE( r.size( ) == 2 );

      // Check if both regions have the right number of vertices together
      REQUIRE( r[0].size( ) + r[1].size( ) == 8 + 2 * n );

      // Check if the indices add up correctly
      CHECK( std::accumulate( r[0].begin( ), r[0].end( ), 0.0 ) +
             std::accumulate( r[1].begin( ), r[1].end( ), 0.0 )
                 == 44 + regions[0][i] + regions[0][j] + 18 * ( n == 3 ) );

      // Check if the area of the two polygons add to the original polygon
      CHECK( polygonArea( v, r[0] ) + polygonArea( v, r[1] ) == Approx( 7.0 ) );

    } // for j
  } // for i

} // divideRegion

TEST_CASE_METHOD( CircularRegionFixture, "divideRegionDiagonal" )
{
  double tolerance = 1e-7;

  for( size_t i = 0; i < 4; ++i )
  {
    auto v = vertices;
    auto r = regions;

    REQUIRE_NOTHROW( meshgeneratorhelper::divideRegion( v, r, 0, i, i + 4, 1.0 ) );
    REQUIRE( r.size( ) == 2 );
    REQUIRE( r[0].size( ) == 7 );
    REQUIRE( r[1].size( ) == 7 );

    CHECK( polygonArea( v, r[0] ) == Approx( 3.5 ) );
    CHECK( polygonArea( v, r[1] ) == Approx( 3.5 ) );

    // check if the length of all line segments are in [1, sqrt(2)]
    for( size_t j = 0; j < 7; ++j )
    {
      // for both regions
      for( const auto& region : r )
      {
        double segmentLength = meshgeneratorhelper::length( meshgeneratorhelper::minus( v[region[j]], v[region[(j + 1) % 7]] ) );

        CHECK( segmentLength > 1.0 - tolerance );
        CHECK( segmentLength < std::sqrt( 2.0 ) + tolerance );
      }
    } // for j
  } // for i

} // divideRegionDiagonal

TEST_CASE( "divideRegionSingleSegment1")
{
  Vertex2DVector vertices{ { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 0.5 }, { 2.0, 0.5 } };
  std::vector<IndexVector> regions{ { 0, 1, 3, 2 } };

  REQUIRE( polygonArea( vertices, regions[0] ) == Approx( 0.5 ) );

  REQUIRE_NOTHROW( meshgeneratorhelper::divideRegion( vertices, regions, 0, 1, 3, 2.0 ) );

  REQUIRE( regions.size( ) == 2 );
  REQUIRE( regions[0].size( ) == 3 );
  REQUIRE( regions[1].size( ) == 3 );

  CHECK( polygonArea( vertices, regions[0] ) + polygonArea( vertices, regions[1] ) == Approx( 0.5 ) );
}

TEST_CASE_METHOD( CircularRegionFixture, "attemptDivision" )
{
  double alpha = std::atan( 1.0 / 3.0 ) + PI / 4.0;
  double delta = 1e-5;

  bool result;

  SECTION( "small angle" )
  {
    REQUIRE_NOTHROW( result = meshgeneratorhelper::attemptDivision( vertices, regions, 0, alpha / 2.0, 1.0 ) );

    CHECK( result == true );
  }

  SECTION( "only just possible" )
  {
    REQUIRE_NOTHROW( result = meshgeneratorhelper::attemptDivision( vertices, regions, 0, alpha - delta, 1.0 ) );

    REQUIRE( regions.size( ) == 2 );

    CHECK( result == true );
    CHECK( regions[0].size( ) == 7 );
    CHECK( regions[1].size( ) == 7 );
    CHECK( vertices.size( ) == 10 );
  }

  SECTION( "only just not possible anymore" )
  {
    REQUIRE_NOTHROW( result = meshgeneratorhelper::attemptDivision( vertices, regions, 0, alpha + delta, 1.0 ) );

    CHECK( result == false );
    CHECK( vertices.size( ) == 8 );
    CHECK( regions.size( ) == 1 );
  }

  SECTION( "large angle" )
  {
    REQUIRE_NOTHROW( result = meshgeneratorhelper::attemptDivision( vertices, regions, 0, 2.0 * alpha, 1.0 ) );

    CHECK( result == false );
    CHECK( vertices.size( ) == 8 );
    CHECK( regions.size( ) == 1 );
  }
} // attemptDivision

TEST_CASE( "attemptDivisionLessThanFourVertices" )
{
  Vertex2DVector vertices{ { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 } };
  std::vector<IndexVector> regions{ { 0, 1, 2 } };

  bool result;

  REQUIRE_NOTHROW( result = meshgeneratorhelper::attemptDivision( vertices, regions, 0, PI / 4.0, 1.0 ) );

  CHECK( result == true );
  CHECK( vertices.size( ) ==  3 );
  REQUIRE( regions.size( ) == 1 );
  CHECK( regions[0].size( ) == 3 );
}

struct ChopTestFixture
{
  /* Construct the following polygon:
   *
   *       4  o
   *         / \
   *     3  o   \
   *       /     \
   *   2  o       \
   *       \       \
   *        o-------o
   *      0           1
   */

  Triangulation triangulation;
  IndexVector region;

  ChopTestFixture( ) :
    triangulation{ { { 2.5, -1.0 }, { 4.0, -1.0 }, { 2.0, -0.5 }, { 2.5, 0.0 }, { 3.0, 0.5 } }, { } },
    region{ 1, 4, 3, 2, 0 }
  { }
};

TEST_CASE_METHOD( ChopTestFixture, "chopQuality" )
{
  std::array<double, 5> computedChopQualities;

  REQUIRE_NOTHROW( computedChopQualities[0] = meshgeneratorhelper::chopQuality( triangulation.first, region, 4, 0, 1 ) );
  REQUIRE_NOTHROW( computedChopQualities[1] = meshgeneratorhelper::chopQuality( triangulation.first, region, 0, 1, 2 ) );
  REQUIRE_NOTHROW( computedChopQualities[2] = meshgeneratorhelper::chopQuality( triangulation.first, region, 1, 2, 3 ) );
  REQUIRE_NOTHROW( computedChopQualities[3] = meshgeneratorhelper::chopQuality( triangulation.first, region, 2, 3, 4 ) );
  REQUIRE_NOTHROW( computedChopQualities[4] = meshgeneratorhelper::chopQuality( triangulation.first, region, 3, 4, 0 ) );

  CHECK( computedChopQualities[0] == Approx( std::sqrt( 10.0 / 13.0 ) ) );
  CHECK( computedChopQualities[1] == Approx( std::sqrt( 2.0 / 13.0 ) ) );
  CHECK( computedChopQualities[2] == Approx( 1.0 / 2.0 ) );
  CHECK( computedChopQualities[3] == Approx( std::sqrt( 2.0 ) / 2.0 ) );
  CHECK( computedChopQualities[4] == Approx( std::sqrt( 2.0 / 17.0 ) ) );
}

bool containsIndices( TriangleConnectivity connectivity, size_t id1, size_t id2, size_t id3 )
{
  return ( connectivity[0] == id1 || connectivity[0] == id2 || connectivity[0] == id3 ) &&
         ( connectivity[1] == id1 || connectivity[1] == id2 || connectivity[1] == id3 ) &&
         ( connectivity[2] == id1 || connectivity[2] == id2 || connectivity[2] == id3 );
}

TEST_CASE_METHOD( ChopTestFixture, "chopTriangle" )
{
  double epsilon = 1e-5;
  int result = true;

  REQUIRE_NOTHROW( result = meshgeneratorhelper::chopTriangle( triangulation, region, std::sqrt( 10.0 / 13.0 ) + epsilon ) );

  CHECK( result == false );

  REQUIRE( triangulation.second.empty( ) );
  REQUIRE( region.size( ) ==  5 );

  REQUIRE_NOTHROW( result = meshgeneratorhelper::chopTriangle( triangulation, region, std::sqrt( 10.0 / 13.0 ) - epsilon ) );

  REQUIRE( triangulation.second.size( ) == 1 );
  REQUIRE( region.size( ) ==  4 );

  CHECK( result == true );
  CHECK( region == IndexVector{ 4, 3, 2, 0 } );
  CHECK( containsIndices( triangulation.second[0], 0, 1, 4 ) );

  REQUIRE_NOTHROW( result = meshgeneratorhelper::chopTriangle( triangulation, region, std::sqrt( 2.0 ) / 2.0 + epsilon ) );

  CHECK( result == false );

  REQUIRE( triangulation.second.size( ) == 1 );
  REQUIRE( region.size( ) ==  4 );

  REQUIRE_NOTHROW( result = meshgeneratorhelper::chopTriangle( triangulation, region, std::sqrt( 2.0 ) / 2.0 - epsilon ) );

  REQUIRE( triangulation.second.size( ) == 2 );
  REQUIRE( region.size( ) ==  3 );

  CHECK( result == true );
  CHECK( region == IndexVector{ 4, 3, 0 } );
  CHECK( containsIndices( triangulation.second[1], 3, 2, 0 ) );

  REQUIRE_NOTHROW( result = meshgeneratorhelper::chopTriangle( triangulation, region, 0.0 ) );

  REQUIRE( triangulation.second.size( ) == 3 );

  CHECK( result == false );
  CHECK( region.size( ) ==  0 );
  CHECK( containsIndices( triangulation.second[2], 4, 3, 0 ) );

  CHECK( triangulation.first.size( ) == 5 );
}

// Helper function that sums up the area of each triangle for test below
double triangulationArea( const Triangulation& triangulation )
{
  double area = 0.0;

  for( const auto& connectivity : triangulation.second )
  {
    auto d1 = meshgeneratorhelper::minus( triangulation.first[connectivity[1]], triangulation.first[connectivity[0]] );
    auto d2 = meshgeneratorhelper::minus( triangulation.first[connectivity[2]], triangulation.first[connectivity[0]] );

    area +=  0.5 * std::abs( d1[0] * d2[1] - d1[1] * d2[0] );
  }

  return area;
} // triangulationArea

TEST_CASE_METHOD( CircularRegionFixture, "triangulate" )
{
  Triangulation triangulation;
  TriangulationParameters parameters;

  REQUIRE_NOTHROW( triangulation = triangulate( vertices, regions, parameters ) );

  const auto& triangulationVertices = triangulation.first;
  const auto& triangulationConnectivity = triangulation.second;

  size_t numberOfVertices = triangulationVertices.size( );
  size_t numberOfTriangles = triangulationConnectivity.size( );

  REQUIRE( numberOfVertices >= 8 );  // That's what we started with
  REQUIRE( numberOfTriangles >= 6 ); // That is the minimum amount of triangles needed

  // Check that maximum vertex index is number of vertices - 1
  for( const auto& connectivity : triangulationConnectivity )
  {
    REQUIRE( *std::max_element( connectivity.begin( ), connectivity.end( ) ) < numberOfVertices );
  }

  CHECK( triangulationArea( triangulation ) == Approx( 7.0 ) );
} // triangulate

} // namespace meshkernel
}
