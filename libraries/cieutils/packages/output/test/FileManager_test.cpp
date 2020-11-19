// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "cieutils/packages/output/inc/FileManager.hpp"
#include "cieutils/packages/output/inc/fileinfo.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <iostream>


namespace cie::utils {


std::string testDir = TEST_OUTPUT_PATH / "test";


TEST_CASE( "FileManager - constructor" )
{
    auto createFileManager = []() ->void 
        {FileManager fileManager(testDir);};
    CHECK_NOTHROW( createFileManager() );

    CHECK_NOTHROW( createFileManager() );
}


TEST_CASE( "FileManager - file handling" )
{
    // Create FileManager
    FileManager manager( testDir );

    // Clear directory
    std::vector<std::string> fileList;
    REQUIRE_NOTHROW( fileList = manager.listFiles() );
    for ( const auto& name : fileList )
        CHECK_NOTHROW( manager.deleteFile( name ) );

    // Create input file
    File& file1     = manager.newFile( "inFile.bin" );
    file1.close();

    // Create files
    File& outFile   = manager.newFile( "outFile.txt" );
    File& inFile    = manager.open( "inFile.bin" );

    // Check if files are open
    CHECK( outFile.is_open() );
    CHECK( inFile.is_open() );

    // List files
    REQUIRE_NOTHROW( fileList = manager.listFiles() );

    // Check file names and validity in directory
    bool result = true;
    for ( const auto& name : fileList )
    {
        CHECK( detail::isFile( testDir + "" + name ) );
        if ( detail::fileName(name)!="outFile.txt" && detail::fileName(name)!="inFile.bin" )
            result = false;
        CHECK( result );
    }

    // Close all files
    CHECK_NOTHROW( manager.closeAll() );

    // Check if files are open
    CHECK( !outFile.is_open() );
    CHECK( !inFile.is_open() );

    // Delete a file
    CHECK_NOTHROW( manager.deleteFile( "outFile.txt" ) );

    // Check if file was deleted
    REQUIRE_NOTHROW( fileList = manager.listFiles() );
    for ( const auto& name : fileList )
    {
        CHECK( detail::isFile( testDir + "" + name ) );
        if ( detail::fileName(name)=="outFile.txt" && detail::fileName(name)!="inFile.bin" )
            result = false;
        CHECK( result );
    }

    // Delete a file
    CHECK_NOTHROW( manager.deleteFile( "inFile.bin" ) );

    // Check if file was deleted - directory should be empty now
    REQUIRE_NOTHROW( fileList = manager.listFiles() );
    CHECK( fileList.size() == 0 );
}


} // namespace cie::detail