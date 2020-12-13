// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include <linalg/linalg.hpp>
#include "linalg/packages/utilities/inc/linalghelper.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <fstream>
#include <sstream>
#include <iostream>

namespace cie::linalg
{

CIE_TEST_CASE( "Matrix", "[matrix]" )
{
    CIE_TEST_CASE_INIT( "Matrix" )

    CHECK( Matrix<Double>( 1, 3, 0.0 ).size1( ) == 1 );
    CHECK( Matrix<Double>( 1, 3, 0.0 ).size2( ) == 3 );
    CHECK( Matrix<Double>( 5, 2 ).sizes( )[0] == 5);
    CHECK( Matrix<Double>( 5, 2 ).sizes( )[1] == 2 );

    Matrix<Double> matrix1( 2, 2, 4.2 );
    Matrix<Double> matrix( matrix1 );

    REQUIRE( matrix.size1( ) == 2 );
    REQUIRE( matrix.size2( ) == 2 );
    REQUIRE( matrix.sizes( )[0] == 2 );
    REQUIRE( matrix.sizes( )[1] == 2 );

    for( Size i = 0; i < 2; ++i )
    {
        for( Size j = 0; j < 2; ++j )
        {
            CHECK( matrix( i, j ) == 4.2 );

            matrix( i, j ) = i * 10.0 + j;
        }
    }

    const Matrix<Double>& ref = matrix;

    for( Size i = 0; i < 2; ++i )
    {
        for( Size j = 0; j < 2; ++j )
        {
            CHECK( ref( i, j ) == i * 10.0 + j );
        }
    }
}

CIE_TEST_CASE( "zero size Matrix", "[matrix]" )
{
    CIE_TEST_CASE_INIT( "zero size Matrix" )

    Matrix<Double> m( 1, 2, 3.0 );

    REQUIRE_NOTHROW( m = Matrix<Double>( 0, 0, 2.0 ) );

    CHECK( m.size1( ) == 0 );
    CHECK( m.size2( ) == 0 );

    REQUIRE_NOTHROW( m = Matrix<Double>{ std::vector<DoubleVector>{ } } );

    CHECK( m.size1( ) == 0 );
    CHECK( m.size2( ) == 0 );
}

CIE_TEST_CASE( "inconsistent input to Matrix", "[matrix]" )
{
    CIE_TEST_CASE_INIT( "inconsistent input to Matrix" )

    std::vector<DoubleVector> inconsistentData
    {
        DoubleVector{  0.0, 0.0, 0.0 },
        DoubleVector{  0.0, 0.0 },
        DoubleVector{  0.0, 0.0, 0.0 }
    };

    CHECK_THROWS_AS( Matrix<Double>{ inconsistentData }, OutOfRangeException );

    CHECK_NOTHROW( Matrix<Double>{ { DoubleVector { } } } );

    CHECK( Matrix<Double>{ { DoubleVector { } } }.size1( ) == 1 );
    CHECK( Matrix<Double>{ { DoubleVector { } } }.size2( ) == 0 );
}


CIE_TEST_CASE( "linearized input to Matrix", "[matrix]" )
{
    CIE_TEST_CASE_INIT( "linearized input to Matrix" )

    std::vector<Double> data{ 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };

    CHECK_THROWS_AS( Matrix<Double>( data, 2 ), OutOfRangeException );

    data.pop_back( );

    REQUIRE_NOTHROW( Matrix<Double>( data, 2 ) );

    Matrix<Double> m( data, 2 );

    CHECK( m( 0, 0 ) == 0.0 );
    CHECK( m( 0, 1 ) == 1.0 );
    CHECK( m( 0, 2 ) == 2.0 );
    CHECK( m( 1, 0 ) == 3.0 );
    CHECK( m( 1, 1 ) == 4.0 );
    CHECK( m( 1, 2 ) == 5.0 );
}

namespace linalgtesthelper
{
    template<typename ContainerType>
    std::vector<Double> writeAndParse( const ContainerType& container )
    {
        const std::filesystem::path testFileName = TEST_OUTPUT_PATH / "Matrix_linearized_input.csv";
        std::ofstream outfile( testFileName );

        REQUIRE_NOTHROW( linalghelper::write( container, outfile ) );

        outfile.close( );

        std::vector<Double> values;

        std::ifstream infile( testFileName );

		std::string line;
        while( !infile.eof() )
        {
			getline(infile, line);
			if (!line.empty() && line!="" && line!="\n")
			{
                std::stringstream ss(line);
                while(getline(ss,line,','))
                    values.push_back( std::stod(line) );
			}
        }

        infile.close( );

        return values;
    }
}

CIE_TEST_CASE( "write vector", "[io]" )
{
    CIE_TEST_CASE_INIT( "write vector" )

    DoubleVector v{ 0.0, 2.0, 4.0 };

    auto result = linalgtesthelper::writeAndParse( v );

    REQUIRE( result.size( ) == 3 );

    CHECK( result[0] == v[0] );
    CHECK( result[1] == v[1] );
    CHECK( result[2] == v[2] );
}

CIE_TEST_CASE( "write matrix", "[io]" )
{
    CIE_TEST_CASE_INIT( "write matrix" )

    Matrix<Double> m( { DoubleVector{ 1.0, -2.0 },
                DoubleVector{ -8.0, 4.0 } } );

    auto result = linalgtesthelper::writeAndParse( m );

    REQUIRE( result.size( ) == 4 );

    CHECK( result[0] == m( 0, 0 ) );
    CHECK( result[1] == m( 0, 1 ) );
    CHECK( result[2] == m( 1, 0 ) );
    CHECK( result[3] == m( 1, 1 ) );
}

CIE_TEST_CASE( "norm", "[utility]" )
{
    CIE_TEST_CASE_INIT( "norm" )

    DoubleVector vector { 4.0, 3.0 };

    Double tolerance = 1e-12;

    CHECK( norm( vector ) == Approx( 5.0 ).epsilon( tolerance ) );

    Matrix<Double> matrix( { DoubleVector{  1.2,  3.3,  9.1 },
                     DoubleVector{  2.9,  8.6,  2.6 },
                     DoubleVector{ -4.8,  0.3, -2.1 } } );

    CHECK( norm( matrix ) == Approx( 14.553693689232297 ).epsilon( tolerance ) );
}

CIE_TEST_CASE( "solve", "[solvers]" )
{
    CIE_TEST_CASE_INIT( "solve" )

    Matrix<Double> matrix( { DoubleVector{  5.2,  1.2,  7.3, -2.3 },
                     DoubleVector{  8.9, -7.6, -0.2,  3.4 },
                     DoubleVector{ -5.7,  6.2, -3.4,  7.8 },
                     DoubleVector{  9.8, -0.7,  5.4, -2.1 } } );

    DoubleVector rhs { -3.8, 0.3, 6.1, 2.8 };

    REQUIRE( matrix.size1( ) == 4 );
    REQUIRE( matrix.size2( ) == 4 );

    DoubleVector solution = solve( matrix, rhs );

    REQUIRE( solution.size( ) == 4 );

    Double tolerance = 1e-12;

    CHECK( solution[0] == Approx(  1.3218868527560599 ).epsilon( tolerance ) );
    CHECK( solution[1] == Approx(  1.4702413909379624 ).epsilon( tolerance ) );
    CHECK( solution[2] == Approx( -1.7634954348963456 ).epsilon( tolerance ) );
    CHECK( solution[3] == Approx( -0.1893110309940256 ).epsilon( tolerance ) );
}

CIE_TEST_CASE( "solve singular", "[solvers]" )
{
    CIE_TEST_CASE_INIT( "solve singular" )

    Matrix<Double> matrix( { DoubleVector{  1.0, -1.0 },
                     DoubleVector{ -1.0,  1.0 } } );

    DoubleVector rhs { 0.0, 0.0 };

    REQUIRE( matrix.size1( ) == 2 );
    REQUIRE( matrix.size2( ) == 2 );

    CHECK_THROWS_AS( solve( matrix, rhs ), MatrixError );
}

CIE_TEST_CASE( "solve zero pivot", "[solvers]" )
{
    CIE_TEST_CASE_INIT( "solve zero pivot" )

    Matrix<Double> matrix( {    DoubleVector{ 0.0       , 1.85943691, 6.97111553, 8.69227093 },
					            DoubleVector{ 3.28991871, 1.53597987, 7.61514532, 3.60326283 },
					            DoubleVector{ 2.94417179, 6.82592967, 1.11991796, 3.83895535 },
					            DoubleVector{ 5.35016312, 6.02883842, 6.90193734, 1.79666592 } } );

    DoubleVector rhs { 2.50464842, 9.42931494, 3.00660354, 1.41937798 };

    REQUIRE( matrix.size1( ) == 4 );
    REQUIRE( matrix.size2( ) == 4 );

    DoubleVector expectedSolution{ 7.57541894507922, -4.311354527696106, -2.8003180763255577, 3.456252253714161 };
    DoubleVector computedSolution;

    REQUIRE_NOTHROW( computedSolution = solve( matrix, rhs ) );
    REQUIRE( computedSolution.size( ) == 4 );

    Double tolerance = 1e-12;

    for( Size i = 0; i < 4; ++i )
    {
    	CHECK( computedSolution[i] == Approx( expectedSolution[i] ).epsilon( tolerance ) );
    }
}

} // namespace cie::linalg