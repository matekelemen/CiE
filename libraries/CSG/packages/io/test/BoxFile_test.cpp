// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "CSG/packages/io/inc/BoxFile.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>
#include <filesystem>


namespace cie::csg {


TEST_CASE( "BoxFile", "[io]" )
{
    CIE_TEST_CASE_INIT( "BoxFile" )

    std::filesystem::path outputPath = TEST_OUTPUT_PATH / "BoxFile";

    const Size Dimension = 2;
    using CoordinateType = Double;
    using BoxType        = Box<Dimension,CoordinateType>;

    const Size numberOfBoxesPerDimension = 5;

    {
        CIE_TEST_CASE_INIT( "write cubes" )

        // Construct cubes
        using PrimitiveType = Cube<Dimension,CoordinateType>;
        std::vector<PrimitiveType> primitives;
        std::vector<std::shared_ptr<PrimitiveType>> primitivePtrs;

        primitives.reserve( numberOfBoxesPerDimension * numberOfBoxesPerDimension );

        for ( Size i=0; i<numberOfBoxesPerDimension; ++i )
            for ( Size j=0; j<numberOfBoxesPerDimension; ++j )
            {
                typename PrimitiveType::point_type base { Double(i)/numberOfBoxesPerDimension, Double(j)/numberOfBoxesPerDimension };
                typename PrimitiveType::coordinate_type length = 1.0 / numberOfBoxesPerDimension;

                primitives.emplace_back( base, length );
                primitivePtrs.emplace_back( new PrimitiveType(base,length) );
            }

        {
            std::filesystem::path fileName = outputPath / "cube0.boxes";
            {
                BoxFile outputFile( fileName,
                                    Dimension,
                                    sizeof(CoordinateType) );
                outputFile << primitives[0];
            }

            BoxFile inputFile( fileName );
            BoxType test;
            inputFile >> test;

            for ( Size dim=0; dim<Dimension; ++dim )
            {
                CHECK( test.base()[dim] == Approx( primitives[0].base()[dim] ) );
                CHECK( test.lengths()[dim] == Approx( primitives[0].length() ) );
            }
        }

        {
            std::filesystem::path fileName = outputPath / "cube1.boxes";
            {
                BoxFile outputFile( fileName,
                                    Dimension,
                                    sizeof(CoordinateType) );
                outputFile << primitives;
            }

            BoxFile inputFile( fileName );
            std::vector<BoxType> test;
            inputFile >> test;

            for ( Size i=0; i<test.size(); ++i )
                for ( Size dim=0; dim<Dimension; ++dim )
                {
                    CHECK( test[i].base()[dim] == Approx( primitives[i].base()[dim] ) );
                    CHECK( test[i].lengths()[dim] == Approx( primitives[i].length() ) );
                }
        }

        {
            std::filesystem::path fileName = outputPath / "cube2.boxes";
            {
                BoxFile outputFile( fileName,
                                    Dimension,
                                    sizeof(CoordinateType) );
                outputFile << primitivePtrs;
            }

            BoxFile inputFile( fileName );
            std::vector<std::shared_ptr<BoxType>> test;
            inputFile >> test;

            for ( Size i=0; i<test.size(); ++i )
                for ( Size dim=0; dim<Dimension; ++dim )
                {
                    CHECK( test[i]->base()[dim] == Approx( primitivePtrs[i]->base()[dim] ) );
                    CHECK( test[i]->lengths()[dim] == Approx( primitivePtrs[i]->length() ) );
                }
        }
    }


    {
        CIE_TEST_CASE_INIT( "write boxes" )
    }
}


} // namespace cie::csg