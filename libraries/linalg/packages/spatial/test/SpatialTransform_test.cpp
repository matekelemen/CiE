#define _USE_MATH_DEFINES

// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "linalg/packages/spatial/inc/SpatialTransform.hpp"

// --- STL Includes ---
#include <cmath>


namespace cie::linalg {


CIE_TEST_CASE( "SpatialTransform", "[spatial]" )
{
    CIE_TEST_CASE_INIT( "SpatialTransform" )

    using Transform = SpatialTransform<double>;

    Transform transform;
    Transform::vector<3> test;

    // Identity
    test = {1, 0, 0};
    CIE_TEST_CHECK_NOTHROW( test = transform.transform( test ) );
    CIE_TEST_CHECK( test[0] == Approx(1.0) );
    CIE_TEST_CHECK( test[1] == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( test[2] == Approx(0.0).margin(1e-14) );

    // Rotatio around z
    CIE_TEST_CHECK_NOTHROW(
        transform = Transform::makeRotation( M_PI / 2.0, Transform::vector<2> {0.0, 0.0} )
    );
    CIE_TEST_CHECK_NOTHROW( test = transform.transform( test ) );
    CIE_TEST_CHECK( test[0] == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( test[1] == Approx(1.0) );
    CIE_TEST_CHECK( test[2] == Approx(0.0).margin(1e-14) );

    // Rotation around a general axis passing through the origin
    CIE_TEST_CHECK_NOTHROW(
        transform = Transform::makeRotation(
            Transform::vector<3> { -1, 0, 1 },
            - M_PI / 2.0,
            Transform::vector<3> { 0, 0, 0 }
        )
    );
    CIE_TEST_CHECK_NOTHROW( test = transform.transform(test) );
    CIE_TEST_CHECK( test[0] == Approx(std::sqrt(2.0) / 2.0) );
    CIE_TEST_CHECK( test[1] == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( test[2] == Approx(std::sqrt(2.0) / 2.0) );

    // Translation
    test = { 1, 0, 0 };
    CIE_TEST_CHECK_NOTHROW(
        transform = Transform::makeTranslation( typename Transform::vector<3> {1, 2, 3} )
    );
    CIE_TEST_CHECK_NOTHROW( test = transform.transform(test) );
    CIE_TEST_CHECK( test[0] == Approx(2.0) );
    CIE_TEST_CHECK( test[1] == Approx(2.0) );
    CIE_TEST_CHECK( test[2] == Approx(3.0) );

    // Rotation around a general axis
    test = { 1, 3, 3 };
    CIE_TEST_CHECK_NOTHROW(
        transform = Transform::makeRotation(
            Transform::vector<3> { -1, 0, 1 },
            - M_PI / 2.0,
            Transform::vector<3> { 1, 2, 3 }
        )
    );
    CIE_TEST_CHECK_NOTHROW( test = transform.transform(test) );
    CIE_TEST_CHECK( test[0] == Approx(1.0 + std::sqrt(2.0) / 2.0) );
    CIE_TEST_CHECK( test[1] == Approx(2.0) );
    CIE_TEST_CHECK( test[2] == Approx(3.0 + std::sqrt(2.0) / 2.0) );

    // Translate, then rotate
    test = {0, 1, 0 };
    CIE_TEST_CHECK_NOTHROW(
        transform = Transform::makeTranslation( Transform::vector<3> {1, 2, 3} )
    );
    CIE_TEST_CHECK_NOTHROW(
        Transform::makeRotation(
            Transform::vector<3> { -1, 0, 1 },
            - M_PI / 2.0,
            Transform::vector<3> { 1, 2, 3 }
        ).transformInPlace( transform )
    );
    CIE_TEST_CHECK_NOTHROW( test = transform.transform(test) );
    CIE_TEST_CHECK( test[0] == Approx(1.0 + std::sqrt(2.0) / 2.0) );
    CIE_TEST_CHECK( test[1] == Approx(2.0) );
    CIE_TEST_CHECK( test[2] == Approx(3.0 + std::sqrt(2.0) / 2.0) );

    // Rotate, then translate
    test = { 0, 1, 0 };
    CIE_TEST_CHECK_NOTHROW(
        transform = Transform::makeRotation( M_PI / 2.0 )
    );
    CIE_TEST_CHECK_NOTHROW(
        Transform::makeTranslation( Transform::vector<3> {1, 2, 3} )
        .transformInPlace(transform)
    );
    CIE_TEST_CHECK_NOTHROW( test = transform.transform(test) );
    CIE_TEST_CHECK( test[0] == Approx(0.0).margin(1e-14) );
    CIE_TEST_CHECK( test[1] == Approx(2.0) );
    CIE_TEST_CHECK( test[2] == Approx(3.0) );
}


} // namespace cie::linalg