// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/output/inc/fileinfo.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <string>
#include <fstream>
#include <filesystem>


namespace cie::utils {
namespace detail {


CIE_TEST_CASE( "File Info", "[output]" )
{
    std::filesystem::path validDirectory      = TEST_OUTPUT_PATH;
    std::filesystem::path invalid             = "abcd/efgh/jklmn/3.145/whatever.cpp/test";

    // Check validity
    CIE_TEST_CHECK( isDirectory(validDirectory) );
    CIE_TEST_CHECK( !isFile(validDirectory) );
    CIE_TEST_CHECK( !isDirectory(invalid) );
    CIE_TEST_CHECK( !isFile(invalid) );

    std::filesystem::path testDirName         = TEST_OUTPUT_PATH / "test";

    std::filesystem::create_directory( testDirName );
    std::filesystem::path testFileBase        = "test_file_name_123";
    std::filesystem::path testFileName        = testDirName / testFileBase;
    std::filesystem::path textExtension       = ".txt";
    std::filesystem::path binaryExtension     = ".bin";
    
    CIE_TEST_CHECK( isTextExtension(textExtension) );
    CIE_TEST_CHECK( isBinaryExtension(binaryExtension) );
    CIE_TEST_CHECK( fileName( testFileBase ) == testFileBase );

    std::filesystem::path textTestName        = testFileName.string() + textExtension.string();
    std::filesystem::path binaryTestName      = testFileName.string() + binaryExtension.string();

    // Check extensions
    CIE_TEST_CHECK( fileExtension(textTestName) == textExtension );
    CIE_TEST_CHECK( fileExtension(textTestName) != binaryExtension );
    CIE_TEST_CHECK( fileExtension(binaryTestName) != textExtension );
    CIE_TEST_CHECK( fileExtension(binaryTestName) == binaryExtension );

    CIE_TEST_CHECK( fileExtension(textTestName) == textExtension );
    CIE_TEST_CHECK( fileExtension(binaryTestName) == binaryExtension );

    // Check file name
    CIE_TEST_CHECK( fileName(textTestName) == testFileBase.string() + textExtension.string() );
    CIE_TEST_CHECK( fileName(binaryTestName) == testFileBase.string() + binaryExtension.string() );

    // Check directory name
    CIE_TEST_CHECK( fileDirectory(textTestName) == testDirName );
    CIE_TEST_CHECK( fileDirectory(binaryTestName) == testDirName );

    // Create files
    std::ofstream textFile( textTestName );
    std::ofstream binaryFile( binaryTestName );

    CIE_TEST_CHECK( textFile.is_open() );
    CIE_TEST_CHECK( binaryFile.is_open() );

    textFile.close();
    binaryFile.close();

    // Test file identification
    CIE_TEST_CHECK( isFile(textTestName) );
    CIE_TEST_CHECK( isFile(binaryTestName) );
    CIE_TEST_CHECK( !isFile(testFileName) );

    CIE_TEST_CHECK( isTextFile(textTestName) );
    CIE_TEST_CHECK( !isTextFile(binaryTestName) );
    CIE_TEST_CHECK( !isTextFile(testFileName) );

    CIE_TEST_CHECK( !isBinaryFile(textTestName) );
    CIE_TEST_CHECK( isBinaryFile(binaryTestName) );
    CIE_TEST_CHECK( !isBinaryFile(testFileName) );

    // Remove text file
    std::filesystem::remove( textTestName );

    CIE_TEST_CHECK( !isFile(textTestName) );
    CIE_TEST_CHECK( isFile(binaryTestName) );
    CIE_TEST_CHECK( !isFile(testFileName) );

    CIE_TEST_CHECK( !isTextFile(textTestName) );
    CIE_TEST_CHECK( !isTextFile(binaryTestName) );
    CIE_TEST_CHECK( !isTextFile(testFileName) );

    CIE_TEST_CHECK( !isBinaryFile(textTestName) );
    CIE_TEST_CHECK( isBinaryFile(binaryTestName) );
    CIE_TEST_CHECK( !isBinaryFile(testFileName) );

    // Remove binary file
    std::filesystem::remove( binaryTestName );

    CIE_TEST_CHECK( !isFile(textTestName) );
    CIE_TEST_CHECK( !isFile(binaryTestName) );
    CIE_TEST_CHECK( !isFile(testFileName) );

    CIE_TEST_CHECK( !isTextFile(textTestName) );
    CIE_TEST_CHECK( !isTextFile(binaryTestName) );
    CIE_TEST_CHECK( !isTextFile(testFileName) );

    CIE_TEST_CHECK( !isBinaryFile(textTestName) );
    CIE_TEST_CHECK( !isBinaryFile(binaryTestName) );
    CIE_TEST_CHECK( !isBinaryFile(testFileName) );
}


} // namespace detail
} // namespace cie::utils