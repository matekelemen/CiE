// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/Loggee.hpp"
#include "../inc/Logger.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <iostream>
#include <string>


namespace cie {
namespace utils {


std::string loggeeTestDir = BINARY_PATH + "/test/loggee";


struct TestLoggee : public Loggee
{
    TestLoggee( Logger& logger ) : Loggee(logger,"TestLoggee") {}
};


TEST_CASE( "Loggee" )
{
    // Create Logger
    Logger logger( loggeeTestDir + "/loggee_test.txt" );

    {
        // Create Loggee
        TestLoggee loggee( logger );
        size_t timerID;

        REQUIRE_NOTHROW( timerID = loggee.tic() );

        CHECK_NOTHROW( loggee.log( "test1" ) );
        CHECK_NOTHROW( loggee.log( "test2", LOG_TYPE_MESSAGE ) );
        CHECK_NOTHROW( loggee.log( "test3", LOG_TYPE_WARNING ) );
        CHECK_THROWS( loggee.log( "test4", LOG_TYPE_ERROR ) );

        CHECK_NOTHROW( loggee.logID( "test5", 0 ) );
        CHECK_NOTHROW( loggee.logID( "test6", 1, LOG_TYPE_MESSAGE ) );
        CHECK_NOTHROW( loggee.logID( "test7", 2, LOG_TYPE_WARNING ) );
        CHECK_THROWS( loggee.logID( "test8", 3, LOG_TYPE_ERROR ) );

        CHECK_NOTHROW( loggee.toc(timerID, false) );
        CHECK_NOTHROW( loggee.toc( "test9", timerID, false) );
        CHECK_NOTHROW( loggee.toc(timerID) );

        CHECK_NOTHROW( loggee.separate() );
        CHECK_NOTHROW( loggee.fileManager() );
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

    REQUIRE_NOTHROW( get() );
    CHECK_NOTHROW( get() );
    CHECK( checkLine("TestLoggee") );
    CHECK( checkLine("[TestLoggee] test1") );
    CHECK( checkLine("[TestLoggee] test2") );
    CHECK( checkLine("WARNING: [TestLoggee] test3") );
    CHECK( checkLine("ERROR: [TestLoggee] test4") );
    CHECK( checkLine("[TestLoggee] test5 | id_0") );
    CHECK( checkLine("[TestLoggee] test6 | id_1") );
    CHECK( checkLine("WARNING: [TestLoggee] test7 | id_2") );
    CHECK( checkLine("ERROR: [TestLoggee] test8 | id_3") );
    CHECK( checkLine("[TestLoggee]") );
    CHECK( checkLine("[TestLoggee] test9") );
    CHECK( checkLine("[TestLoggee]") );

    
    //std::string msg;
    //while (std::getline(file, msg))
    //    std::cout << msg << "\n";
}


}
}