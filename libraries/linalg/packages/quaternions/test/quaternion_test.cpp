// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "linalg/packages/quaternions/inc/quaternion.hpp"

// --- STL Includes ---
#include <memory>

namespace cie::linalg {


CIE_TEST_CASE( "Quaternion", "[quaternions]" )
{
    std::unique_ptr<Quaternion> q;

    CIE_TEST_REQUIRE_NOTHROW( q = std::make_unique<Quaternion>() );
    CIE_TEST_REQUIRE( q->coefficients().size() == 4 );
    CIE_TEST_CHECK( (*q)[0] == Approx(1.0) );
    CIE_TEST_CHECK( (*q)[1] == Approx(0.0) );
    CIE_TEST_CHECK( (*q)[2] == Approx(0.0) );
    CIE_TEST_CHECK( (*q)[3] == Approx(0.0) );

    CIE_TEST_REQUIRE_NOTHROW( q.reset( new Quaternion({0.0,1.0,2.0,3.0}) ) );
    CIE_TEST_REQUIRE( q->coefficients().size() == 4 );
    CIE_TEST_CHECK( (*q)[0] == Approx(0.0) );
    CIE_TEST_CHECK( (*q)[1] == Approx(1.0) );
    CIE_TEST_CHECK( (*q)[2] == Approx(2.0) );
    CIE_TEST_CHECK( (*q)[3] == Approx(3.0) );

    CIE_TEST_REQUIRE_NOTHROW( q.reset( new Quaternion(*q) ) );
    CIE_TEST_REQUIRE( q->coefficients().size() == 4 );
    CIE_TEST_CHECK( (*q)[0] == Approx(0.0) );
    CIE_TEST_CHECK( (*q)[1] == Approx(1.0) );
    CIE_TEST_CHECK( (*q)[2] == Approx(2.0) );
    CIE_TEST_CHECK( (*q)[3] == Approx(3.0) );

    Quaternion p = *q;
    CIE_TEST_REQUIRE( p.coefficients().size() == 4 );
    CIE_TEST_CHECK( p[0] == Approx(0.0) );
    CIE_TEST_CHECK( p[1] == Approx(1.0) );
    CIE_TEST_CHECK( p[2] == Approx(2.0) );
    CIE_TEST_CHECK( p[3] == Approx(3.0) );

    CIE_TEST_CHECK( p.begin() == p.coefficients().begin() );
    CIE_TEST_CHECK( p.end() == p.coefficients().end() );
}


CIE_TEST_CASE( "Quaternion operator overloads", "[quaternions]" )
{
    Quaternion q( {0.0,1.0,2.0,3.0} );
    Quaternion p( {10.0,9.0,8.0,7.0} );
    Quaternion r = q + p;

    for (size_t i=0; i<4; ++i)
        CIE_TEST_CHECK( r[i] == Approx(10.0) );

    r = q - p;
    for (size_t i=0; i<4; ++i)
        CIE_TEST_CHECK( r[i] == Approx( 2*i-10.0 ) );

    r = 2*q;
    for (size_t i=0; i<4; ++i)
        CIE_TEST_CHECK( r[i] == Approx( 2*i ) );

    r = q*2;
    for (size_t i=0; i<4; ++i)
        CIE_TEST_CHECK( r[i] == Approx( 2*i ) );

    r = conjugate(p);
    CIE_TEST_CHECK( r[0] == Approx(p[0]) );
    for (size_t i=1; i<4; ++i)
        CIE_TEST_CHECK( r[i] == Approx( -p[i] ) );
}


} // namespace cie::linalg