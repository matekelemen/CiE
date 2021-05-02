// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "linalg/packages/quaternions/inc/quaternion.hpp"

// --- STL Includes ---
#include <memory>

namespace cie::linalg {



CIE_TEST_CASE( "Quaternion", "[quaternions]" )
{
    std::unique_ptr<Quaternion<double>> p_quaternion;

    CIE_TEST_REQUIRE_NOTHROW( p_quaternion = std::make_unique<Quaternion<double>>() );
    CIE_TEST_REQUIRE( p_quaternion->components().size() == 4 );
    CIE_TEST_CHECK( (*p_quaternion)[0] == Approx(1.0) );
    CIE_TEST_CHECK( (*p_quaternion)[1] == Approx(0.0) );
    CIE_TEST_CHECK( (*p_quaternion)[2] == Approx(0.0) );
    CIE_TEST_CHECK( (*p_quaternion)[3] == Approx(0.0) );

    CIE_TEST_REQUIRE_NOTHROW( p_quaternion.reset( new Quaternion<double>({0.0,1.0,2.0,3.0}) ) );
    CIE_TEST_REQUIRE( p_quaternion->components().size() == 4 );
    CIE_TEST_CHECK( (*p_quaternion)[0] == Approx(0.0) );
    CIE_TEST_CHECK( (*p_quaternion)[1] == Approx(1.0) );
    CIE_TEST_CHECK( (*p_quaternion)[2] == Approx(2.0) );
    CIE_TEST_CHECK( (*p_quaternion)[3] == Approx(3.0) );

    CIE_TEST_REQUIRE_NOTHROW( p_quaternion.reset( new Quaternion<double>(*p_quaternion) ) );
    CIE_TEST_REQUIRE( p_quaternion->components().size() == 4 );
    CIE_TEST_CHECK( (*p_quaternion)[0] == Approx(0.0) );
    CIE_TEST_CHECK( (*p_quaternion)[1] == Approx(1.0) );
    CIE_TEST_CHECK( (*p_quaternion)[2] == Approx(2.0) );
    CIE_TEST_CHECK( (*p_quaternion)[3] == Approx(3.0) );

    Quaternion<double> p = *p_quaternion;
    CIE_TEST_REQUIRE( p.components().size() == 4 );
    CIE_TEST_CHECK( p[0] == Approx(0.0) );
    CIE_TEST_CHECK( p[1] == Approx(1.0) );
    CIE_TEST_CHECK( p[2] == Approx(2.0) );
    CIE_TEST_CHECK( p[3] == Approx(3.0) );

    CIE_TEST_CHECK( p.begin() == p.components().begin() );
    CIE_TEST_CHECK( p.end() == p.components().end() );
}


CIE_TEST_CASE( "Quaternion operator overloads", "[quaternions]" )
{
    Quaternion<double> q( {0.0,1.0,2.0,3.0} );
    Quaternion<double> p( {10.0,9.0,8.0,7.0} );
    Quaternion<double> r = q + p;

    for (size_t i=0; i<4; ++i)
        CIE_TEST_CHECK( r[i] == Approx(10.0) );

    r = q - p;
    for (size_t i=0; i<4; ++i)
        CIE_TEST_CHECK( r[i] == Approx( 2*i-10.0 ) );

    r = p;
    r.conjugate();
    CIE_TEST_CHECK( r[0] == Approx(p[0]) );
    for (size_t i=1; i<4; ++i)
        CIE_TEST_CHECK( r[i] == Approx( -p[i] ) );
}


} // namespace cie::linalg