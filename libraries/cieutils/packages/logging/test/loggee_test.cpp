// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/logging/inc/Loggee.hpp"
#include "cieutils/packages/logging/inc/Logger.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <iostream>
#include <string>
#include <vector>


namespace cie::utils {


std::filesystem::path loggeeTestDir = TEST_OUTPUT_PATH;


struct TestLoggee : public Loggee
{
    TestLoggee( Logger& logger ) : Loggee(logger,"TestLoggee") {}
};


CIE_TEST_CASE( "Loggee", "[logging]" )
{
    CIE_TEST_CASE_INIT( "Loggee" )

    // Create Logger
    Logger logger( loggeeTestDir / "loggee_test.txt",
                   true );

    {
        // Create Loggee
        TestLoggee loggee( logger );
        size_t timerID;

        CIE_TEST_REQUIRE_NOTHROW( timerID = loggee.tic() );

        CIE_TEST_CHECK_NOTHROW( loggee.log( "test1" ) );
        CIE_TEST_CHECK_NOTHROW( loggee.log( "test2", LOG_TYPE_MESSAGE ) );
        CIE_TEST_CHECK_NOTHROW( loggee.log( "test3", LOG_TYPE_WARNING ) );
        CIE_TEST_CHECK_THROWS( loggee.log( "test4", LOG_TYPE_ERROR ) );

        CIE_TEST_CHECK_NOTHROW( loggee.logID( "test5", 0 ) );
        CIE_TEST_CHECK_NOTHROW( loggee.logID( "test6", 1, LOG_TYPE_MESSAGE ) );
        CIE_TEST_CHECK_NOTHROW( loggee.logID( "test7", 2, LOG_TYPE_WARNING ) );
        CIE_TEST_CHECK_THROWS( loggee.logID( "test8", 3, LOG_TYPE_ERROR ) );

        CIE_TEST_CHECK_NOTHROW( loggee.toc(timerID, false) );
        CIE_TEST_CHECK_NOTHROW( loggee.toc( "test9", timerID, false) );
        CIE_TEST_CHECK_NOTHROW( loggee.toc(timerID) );

        CIE_TEST_CHECK_NOTHROW( loggee << "test10" );
        CIE_TEST_CHECK_NOTHROW( loggee << 11 );
        CIE_TEST_CHECK_NOTHROW( loggee << std::vector<int>{ 12, 13 } );

        CIE_TEST_CHECK_NOTHROW( loggee.separate() );
        CIE_TEST_CHECK_NOTHROW( loggee.fileManager() );
    }

    File& file      = logger.fileManager().open( "loggee_test.txt" );
    std::string msg;

    // Define functors to help with testing
    auto get        = [&file, &msg](){ std::getline(file,msg);return msg; };
    auto isSubstr   = [](   const std::string& sub, 
                            const std::string& str) -> bool
        { return str.compare(sub) <= (int)str.size() - (int)sub.size(); };

    auto checkLine  = [&get,&isSubstr,&msg](const std::string& test)
        { get(); return isSubstr(test,msg); };

    CIE_TEST_REQUIRE_NOTHROW( get() );
    CIE_TEST_CHECK_NOTHROW( get() );
    CIE_TEST_CHECK( checkLine("TestLoggee") );
    CIE_TEST_CHECK( checkLine("[TestLoggee] test1") );
    CIE_TEST_CHECK( checkLine("[TestLoggee] test2") );
    CIE_TEST_CHECK( checkLine("WARNING: [TestLoggee] test3") );
    CIE_TEST_CHECK( checkLine("ERROR: [TestLoggee] test4") );
    CIE_TEST_CHECK( checkLine("[TestLoggee] test5 | id_0") );
    CIE_TEST_CHECK( checkLine("[TestLoggee] test6 | id_1") );
    CIE_TEST_CHECK( checkLine("WARNING: [TestLoggee] test7 | id_2") );
    CIE_TEST_CHECK( checkLine("ERROR: [TestLoggee] test8 | id_3") );
    CIE_TEST_CHECK( checkLine("[TestLoggee]") );
    CIE_TEST_CHECK( checkLine("[TestLoggee] test9") );
    CIE_TEST_CHECK( checkLine("[TestLoggee]") );
}


}