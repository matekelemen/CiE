// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "cieutils/packages/output/inc/fileinfo.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <string>
#include <fstream>
#include <filesystem>


namespace cie::utils {
namespace detail {


TEST_CASE( "File Info" )
{
    std::string validDirectory      = TEST_OUTPUT_PATH;
    std::string invalid             = "abcd/efgh/jklmn/3.145/whatever.cpp/test";

    // Check validity
    CHECK( isDirectory(validDirectory) );
    CHECK( !isFile(validDirectory) );
    CHECK( !isDirectory(invalid) );
    CHECK( !isFile(invalid) );

    std::string testDirName         = TEST_OUTPUT_PATH + "/test";

    std::filesystem::create_directory( testDirName );
    std::string testFileBase        = "test_file_name_123";
    std::string testFileName        = testDirName + "/" + testFileBase;
    std::string textExtension       = "txt";
    std::string binaryExtension     = "bin";
    
    CHECK( isTextExtension(textExtension) );
    CHECK( isBinaryExtension(binaryExtension) );
    CHECK( fileName( testFileBase ) == testFileBase );

    std::string textTestName        = testFileName + "." + textExtension;
    std::string binaryTestName      = testFileName + "." + binaryExtension;

    // Check extensions
    CHECK( fileExtension(textTestName) == textExtension );
    CHECK( fileExtension(textTestName) != binaryExtension );
    CHECK( fileExtension(binaryTestName) != textExtension );
    CHECK( fileExtension(binaryTestName) == binaryExtension );

    CHECK( fileExtension(textExtension) == textExtension );
    CHECK( fileExtension(binaryExtension) == binaryExtension );

    // Check file name
    CHECK( fileName(textTestName) == testFileBase + "." + textExtension );
    CHECK( fileName(binaryTestName) == testFileBase + "." + binaryExtension );

    // Check directory name
    CHECK( fileDirectory(textTestName) == testDirName );
    CHECK( fileDirectory(binaryTestName) == testDirName );

    // Create files
    std::ofstream textFile( textTestName );
    std::ofstream binaryFile( binaryTestName );

    CHECK( textFile.is_open() );
    CHECK( binaryFile.is_open() );

    textFile.close();
    binaryFile.close();

    // Test file identification
    CHECK( isFile(textTestName) );
    CHECK( isFile(binaryTestName) );
    CHECK( !isFile(testFileName) );

    CHECK( isTextFile(textTestName) );
    CHECK( !isTextFile(binaryTestName) );
    CHECK( !isTextFile(testFileName) );

    CHECK( !isBinaryFile(textTestName) );
    CHECK( isBinaryFile(binaryTestName) );
    CHECK( !isBinaryFile(testFileName) );

    // Remove text file
    std::filesystem::remove( textTestName );

    CHECK( !isFile(textTestName) );
    CHECK( isFile(binaryTestName) );
    CHECK( !isFile(testFileName) );

    CHECK( !isTextFile(textTestName) );
    CHECK( !isTextFile(binaryTestName) );
    CHECK( !isTextFile(testFileName) );

    CHECK( !isBinaryFile(textTestName) );
    CHECK( isBinaryFile(binaryTestName) );
    CHECK( !isBinaryFile(testFileName) );

    // Remove binary file
    std::filesystem::remove( binaryTestName );

    CHECK( !isFile(textTestName) );
    CHECK( !isFile(binaryTestName) );
    CHECK( !isFile(testFileName) );

    CHECK( !isTextFile(textTestName) );
    CHECK( !isTextFile(binaryTestName) );
    CHECK( !isTextFile(testFileName) );

    CHECK( !isBinaryFile(textTestName) );
    CHECK( !isBinaryFile(binaryTestName) );
    CHECK( !isBinaryFile(testFileName) );
}


} // namespace detail
} // namespace cie::utils