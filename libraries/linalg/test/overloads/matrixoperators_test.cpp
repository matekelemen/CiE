#include "catch.hpp"
#include "../../overloads/inc/matrixoperators.hpp"

namespace cie {
namespace linalg {

TEST_CASE("Matrix - scalar operator overloads"){
    Matrix matrix( 
        {
            0.0, 1.1, 2.2,
            3.3, 4.4, 5.5 
        }, 2
        );
    Matrix result;
    double scalar = 2.5;

    // ADDITION
    REQUIRE_NOTHROW( result = matrix + scalar );
    REQUIRE( result.size1() == matrix.size1() );
    REQUIRE( result.size2() == matrix.size2() );
    for (size_t i=0; i<result.size1(); ++i){
        for (size_t j=0; j<result.size2(); ++j){
            CHECK( result(i,j) == matrix(i,j) + scalar );
        }
    }

    REQUIRE_NOTHROW( result = scalar + matrix );
    REQUIRE( result.size1() == matrix.size1() );
    REQUIRE( result.size2() == matrix.size2() );
    for (size_t i=0; i<result.size1(); ++i){
        for (size_t j=0; j<result.size2(); ++j){
            CHECK( result(i,j) == matrix(i,j) + scalar );
        }
    }

    // SUBTRACTION
    REQUIRE_NOTHROW( result = matrix - scalar );
    REQUIRE( result.size1() == matrix.size1() );
    REQUIRE( result.size2() == matrix.size2() );
    for (size_t i=0; i<result.size1(); ++i){
        for (size_t j=0; j<result.size2(); ++j){
            CHECK( result(i,j) == matrix(i,j) - scalar );
        }
    }
    
    // MULTIPLICATION
    REQUIRE_NOTHROW( result = matrix * scalar );
    REQUIRE( result.size1() == matrix.size1() );
    REQUIRE( result.size2() == matrix.size2() );
    for (size_t i=0; i<result.size1(); ++i){
        for (size_t j=0; j<result.size2(); ++j){
            CHECK( result(i,j) == matrix(i,j) * scalar );
        }
    }

    REQUIRE_NOTHROW( result = scalar * matrix );
    REQUIRE( result.size1() == matrix.size1() );
    REQUIRE( result.size2() == matrix.size2() );
    for (size_t i=0; i<result.size1(); ++i){
        for (size_t j=0; j<result.size2(); ++j){
            CHECK( result(i,j) == matrix(i,j) * scalar );
        }
    }

} // TEST_CASE


TEST_CASE("Matrix - vector operator overloads"){
    Matrix matrix( 
        {
            0.0, 1.1, 2.2,
            3.3, 4.4, 5.5 
        }, 2
        );
    DoubleVector v1 = {0.2, 0.5};
    DoubleVector v2 = {-0.5, 1.6, 3.5};
    DoubleVector v3 = {5.0, -5.0, 1.0, 3.0, 12.0};
    DoubleVector result;

    REQUIRE_NOTHROW( result = v1 * matrix);
    REQUIRE( result.size() == matrix.size2() );
    CHECK( result[0] == v1[0]*matrix(0,0) + v1[1]*matrix(1,0) );
    CHECK( result[1] == v1[0]*matrix(0,1) + v1[1]*matrix(1,1) );
    CHECK( result[2] == v1[0]*matrix(0,2) + v1[1]*matrix(1,2) );
    REQUIRE_THROWS( result = matrix * v1 );

    REQUIRE_NOTHROW( result = matrix * v2 );
    REQUIRE( result.size() == matrix.size1() );
    CHECK( result[0] == v2[0]*matrix(0,0) + v2[1]*matrix(0,1) + v2[2]*matrix(0,2) );
    CHECK( result[1] == v2[0]*matrix(1,0) + v2[1]*matrix(1,1) + v2[2]*matrix(1,2) );
    REQUIRE_THROWS( result = v2 * matrix );

    REQUIRE_THROWS( result = matrix * v3 );
    REQUIRE_THROWS( result = v3 * matrix );

} // TEST_CASE


TEST_CASE("Matrix - matrix operator overloads"){
    Matrix m2x3( 
        {
            0.0, 1.1, 2.2,
            3.3, 4.4, 5.5 
        }, 2
        );
    Matrix M2x3 = m2x3*10.0;
    Matrix m2x2(
        {
            4.5, 4.6,
            3.1, 3.0
        }, 2
    );
    Matrix m3x3(
        {
            10.0, 20.0, 30.0,
            40.0, 50.0, 60.0, 
            70.0, 80.0, 90.0
        }, 3
    );
    Matrix m4x4(4,4,0.0);
    Matrix result;

    // ADDITION
    REQUIRE_NOTHROW( result = m2x3 + M2x3 );
    REQUIRE( result.size1() == m2x3.size1() );
    REQUIRE( result.size2() == m2x3.size2() );
    for (size_t i=0; i<result.size1(); ++i){
        for (size_t j=0; j<result.size2(); ++j){
            CHECK( result(i,j) == m2x3(i,j) + M2x3(i,j) );
        }
    }
    REQUIRE_THROWS( result = m2x3 + m3x3 );
    REQUIRE_THROWS( result = m2x2 + m2x3 );

    // SUBTRACTION
    REQUIRE_NOTHROW( result = m2x3 - M2x3 );
    REQUIRE( result.size1() == m2x3.size1() );
    REQUIRE( result.size2() == m2x3.size2() );
    for (size_t i=0; i<result.size1(); ++i){
        for (size_t j=0; j<result.size2(); ++j){
            CHECK( result(i,j) == m2x3(i,j) - M2x3(i,j) );
        }
    }
    REQUIRE_THROWS( result = m2x3 - m3x3 );
    REQUIRE_THROWS( result = m2x2 - m2x3 );

    // MULTIPLICATION
    REQUIRE_NOTHROW( result = m2x3 * m3x3 );
    REQUIRE( result.size1() == m2x3.size1() );
    REQUIRE( result.size2() == m3x3.size2() );
    CHECK( result(0,0) == m2x3(0,0)*m3x3(0,0) + m2x3(0,1)*m3x3(1,0) + m2x3(0,2)*m3x3(2,0) );
    CHECK( result(0,1) == m2x3(0,0)*m3x3(0,1) + m2x3(0,1)*m3x3(1,1) + m2x3(0,2)*m3x3(2,1) );
    CHECK( result(0,2) == m2x3(0,0)*m3x3(0,2) + m2x3(0,1)*m3x3(1,2) + m2x3(0,2)*m3x3(2,2) );
    CHECK( result(1,0) == m2x3(1,0)*m3x3(0,0) + m2x3(1,1)*m3x3(1,0) + m2x3(1,2)*m3x3(2,0) );
    CHECK( result(1,1) == m2x3(1,0)*m3x3(0,1) + m2x3(1,1)*m3x3(1,1) + m2x3(1,2)*m3x3(2,1) );
    CHECK( result(1,2) == m2x3(1,0)*m3x3(0,2) + m2x3(1,1)*m3x3(1,2) + m2x3(1,2)*m3x3(2,2) );
    REQUIRE_THROWS( result = m3x3 * m2x3 );
    REQUIRE_THROWS( result = m2x3 * m4x4 );
    REQUIRE_THROWS( result = m4x4 * m2x3 );
}

}
}