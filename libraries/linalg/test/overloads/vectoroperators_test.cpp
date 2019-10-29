#include "catch.hpp"
#include "../../overloads/inc/vectoroperators.hpp"

namespace linalg {

TEST_CASE("Vector operator overloads"){
    Vector v1({ 0.0, 1.0, 2.0 });
    Vector v2({-5.0, 0.0, 5.0 });
    Vector v3({ 10.0, 15.0, 14.0, 12.0});
    Vector resultVector;
    double scalar = 2.5;
    double resultScalar;

    // SCALAR ADDITION
    REQUIRE_NOTHROW( resultVector = v1 + scalar );
    REQUIRE( resultVector.size() == v1.size() );
    for (size_t i=0; i<resultVector.size(); ++i){
        CHECK( resultVector[i]== v1[i] + scalar );
    }

    REQUIRE_NOTHROW( resultVector = scalar + v1 );
    REQUIRE( resultVector.size() == v1.size() );
    for (size_t i=0; i<resultVector.size(); ++i){
        CHECK( resultVector[i]== v1[i] + scalar );
    }

    // SCALAR SUBTRACTION
    REQUIRE_NOTHROW( resultVector = v1 - scalar );
    REQUIRE( resultVector.size() == v1.size() );
    for (size_t i=0; i<resultVector.size(); ++i){
        CHECK( resultVector[i]== v1[i] - scalar );
    }

    // SCALAR MULTIPLICATION
    REQUIRE_NOTHROW( resultVector = v1 * scalar );
    REQUIRE( resultVector.size() == v1.size() );
    for (size_t i=0; i<resultVector.size(); ++i){
        CHECK( resultVector[i]== v1[i] * scalar );
    }

    REQUIRE_NOTHROW( resultVector = scalar * v1 );
    REQUIRE( resultVector.size() == v1.size() );
    for (size_t i=0; i<resultVector.size(); ++i){
        CHECK( resultVector[i]== v1[i] * scalar );
    }

    // SCALAR DIVISION
    REQUIRE_THROWS( resultVector = v1 / 0.0 );
    REQUIRE_NOTHROW( resultVector = v1 / scalar );
    REQUIRE( resultVector.size() == v1.size() );
    for (size_t i=0; i<resultVector.size(); ++i){
        CHECK( resultVector[i]== v1[i] / scalar );
    }

    // VECTOR ADDITION
    REQUIRE_THROWS( resultVector = v1 + v3 );
    REQUIRE_NOTHROW( resultVector = v1 + v2 );
    REQUIRE( resultVector.size() == v1.size() );
    for (size_t i=0; i<resultVector.size(); ++i){
        CHECK( resultVector[i]== v1[i] + v2[i] );
    }

    // VECTOR SUBTRACTION
    REQUIRE_THROWS( resultVector = v1 - v3 );
    REQUIRE_NOTHROW( resultVector = v1 - v2 );
    REQUIRE( resultVector.size() == v1.size() );
    for (size_t i=0; i<resultVector.size(); ++i){
        CHECK( resultVector[i]== v1[i] - v2[i] );
    }

    // DOT PRODUCT
    REQUIRE_THROWS( resultScalar = v1 * v3 );
    REQUIRE_NOTHROW( resultScalar = v1 * v2 );
    CHECK( resultScalar == v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2] );

}

}