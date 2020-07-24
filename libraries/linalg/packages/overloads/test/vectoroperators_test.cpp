#include "catch.hpp"
#include "../inc/vectoroperators.hpp"

namespace cie {

TEST_CASE("DoubleVector operator overloads"){
    DoubleVector v1({ 0.0, 1.0, 2.0 });
    DoubleVector v2({-5.0, 0.0, 5.0 });
    DoubleVector v3({ 10.0, 15.0, 14.0, 12.0});
    DoubleVector resultDoubleVector;
    double scalar = 2.5;
    double resultScalar;

    // SCALAR ADDITION
    REQUIRE_NOTHROW( resultDoubleVector = v1 + scalar );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] + scalar );
    }

    REQUIRE_NOTHROW( resultDoubleVector = scalar + v1 );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] + scalar );
    }

    // SCALAR SUBTRACTION
    REQUIRE_NOTHROW( resultDoubleVector = v1 - scalar );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] - scalar );
    }

    // SCALAR MULTIPLICATION
    REQUIRE_NOTHROW( resultDoubleVector = v1 * scalar );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] * scalar );
    }

    REQUIRE_NOTHROW( resultDoubleVector = scalar * v1 );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] * scalar );
    }

    // SCALAR DIVISION
    REQUIRE_THROWS( resultDoubleVector = v1 / 0.0 );
    REQUIRE_NOTHROW( resultDoubleVector = v1 / scalar );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] / scalar );
    }

    // VECTOR ADDITION
    REQUIRE_THROWS( resultDoubleVector = v1 + v3 );
    REQUIRE_NOTHROW( resultDoubleVector = v1 + v2 );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] + v2[i] );
    }

    // VECTOR SUBTRACTION
    REQUIRE_THROWS( resultDoubleVector = v1 - v3 );
    REQUIRE_NOTHROW( resultDoubleVector = v1 - v2 );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] - v2[i] );
    }

    // DOT PRODUCT
    REQUIRE_THROWS( resultScalar = v1 * v3 );
    REQUIRE_NOTHROW( resultScalar = v1 * v2 );
    CHECK( resultScalar == v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2] );
}


TEST_CASE("DoubleArray operator overloads"){
    DoubleArray<3> v1   = { 0.0, 1.0, 2.0 };
    DoubleArray<3> v2   = {-5.0, 0.0, 5.0 };
    DoubleArray<3> resultDoubleVector;
    double scalar = 2.5;
    double resultScalar;

    // SCALAR ADDITION
    REQUIRE_NOTHROW( resultDoubleVector = v1 + scalar );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] + scalar );
    }

    REQUIRE_NOTHROW( resultDoubleVector = scalar + v1 );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] + scalar );
    }

    // SCALAR SUBTRACTION
    REQUIRE_NOTHROW( resultDoubleVector = v1 - scalar );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] - scalar );
    }

    // SCALAR MULTIPLICATION
    REQUIRE_NOTHROW( resultDoubleVector = v1 * scalar );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] * scalar );
    }

    REQUIRE_NOTHROW( resultDoubleVector = scalar * v1 );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] * scalar );
    }

    // SCALAR DIVISION
    REQUIRE_THROWS( resultDoubleVector = v1 / 0.0 );
    REQUIRE_NOTHROW( resultDoubleVector = v1 / scalar );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] / scalar );
    }

    // VECTOR ADDITION
    REQUIRE_NOTHROW( resultDoubleVector = v1 + v2 );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] + v2[i] );
    }

    // VECTOR SUBTRACTION
    REQUIRE_NOTHROW( resultDoubleVector = v1 - v2 );
    REQUIRE( resultDoubleVector.size() == v1.size() );
    for (size_t i=0; i<resultDoubleVector.size(); ++i){
        CHECK( resultDoubleVector[i]== v1[i] - v2[i] );
    }

    // DOT PRODUCT
    REQUIRE_NOTHROW( resultScalar = v1 * v2 );
    CHECK( resultScalar == v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2] );
}

}