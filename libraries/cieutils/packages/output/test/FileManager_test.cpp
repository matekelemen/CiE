// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/output/inc/FileManager.hpp"
#include "cieutils/packages/output/inc/fileinfo.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <iostream>


namespace cie::utils {


std::filesystem::path testDir = TEST_OUTPUT_PATH / "test";


CIE_TEST_CASE( "FileManager - constructor", "[output]" )
{
    auto createFileManager = []() ->void 
        {FileManager fileManager(testDir);};
    CIE_TEST_CHECK_NOTHROW( createFileManager() );

    CIE_TEST_CHECK_NOTHROW( createFileManager() );
}


CIE_TEST_CASE( "FileManager - file handling", "[output]" )
{
    // Create FileManager
    FileManager manager( testDir );

    // Clear directory
    std::vector<std::filesystem::path> fileList;
    CIE_TEST_REQUIRE_NOTHROW( fileList = manager.listFiles() );
    for ( const auto& name : fileList )
        CIE_TEST_CHECK_NOTHROW( manager.deleteFile( name ) );

    // Create input file
    File& file1     = manager.newFile( "inFile.bin" );
    file1.close();

    // Create files
    File& outFile   = manager.newFile( "outFile.txt" );
    File& inFile    = manager.open( "inFile.bin" );

    // Check if files are open
    CIE_TEST_CHECK( outFile.is_open() );
    CIE_TEST_CHECK( inFile.is_open() );

    // List files
    CIE_TEST_REQUIRE_NOTHROW( fileList = manager.listFiles() );

    // Check file names and validity in directory
    bool result;
    for ( const auto& r_name : fileList )
    {
        CIE_TEST_CHECK( detail::isFile( testDir / r_name ) );
        result = true;

        if (detail::fileName(r_name) != "outFile.txt" && detail::fileName(r_name) != "inFile.bin")
            result = false;

        CIE_TEST_CHECK( result );
    }

    // Close all files
    CIE_TEST_CHECK_NOTHROW( manager.closeAll() );

    // Check if files are open
    CIE_TEST_CHECK( !outFile.is_open() );
    CIE_TEST_CHECK( !inFile.is_open() );

    // Delete a file
    CIE_TEST_CHECK_NOTHROW( manager.deleteFile( "outFile.txt" ) );

    // Check if file was deleted
    CIE_TEST_REQUIRE_NOTHROW( fileList = manager.listFiles() );
    for ( const auto& r_name : fileList )
    {
        result = true;

        CIE_TEST_CHECK( detail::isFile( testDir / r_name ) );
        if ( detail::fileName( r_name )=="outFile.txt" && detail::fileName( r_name )!="inFile.bin" )
            result = false;
        CIE_TEST_CHECK( result );
    }

    // Delete a file
    CIE_TEST_CHECK_NOTHROW( manager.deleteFile( "inFile.bin" ) );

    // Check if file was deleted - directory should be empty now
    CIE_TEST_REQUIRE_NOTHROW( fileList = manager.listFiles() );
    CIE_TEST_CHECK( fileList.size() == 0 );
}


} // namespace cie::detail