// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/Logger.hpp"
#include "cmake_variables.hpp"


namespace cie {
namespace utils {


std::string loggerTestDir = BINARY_PATH;


TEST_CASE( "Logger" )
{
    {
        // Construct logger
        Logger logger( loggerTestDir + "/testLog.txt" );

        CHECK_NOTHROW( logger.log( "test1" ) );
        CHECK_NOTHROW( logger.warn( "test2" ) );
        CHECK_THROWS( logger.error( "test3" ) );
        CHECK_NOTHROW( logger.separate( ) );
        CHECK_NOTHROW( logger.increaseIndent() );
        CHECK_NOTHROW( logger.log( "test4" ) );
        CHECK_NOTHROW( logger.increaseIndent() );
        CHECK_NOTHROW( logger.increaseIndent() );
        CHECK_NOTHROW( logger.log( "test5" ) );
        CHECK_NOTHROW( logger.decreaseIndent() );
        CHECK_NOTHROW( logger.log( "test6" ) );
        size_t timerID;
        CHECK_NOTHROW( timerID = logger.startTimer() );
        CHECK_NOTHROW( logger.noIndent() );
        CHECK_NOTHROW( logger.logElapsed( "test7", timerID ) );
    }

    // Create manager and open test log
    FileManager manager( loggerTestDir );
    File& log = manager.open( "testLog.txt" );
    std::string msg;

    // Check lines
    auto get = [&log, &msg](){std::getline( log, msg ); return msg;};
    
    REQUIRE_NOTHROW( get() );
    CHECK( get() == "" );
    CHECK( get() == "test1" );
    CHECK( get() == "WARNING: test2" );
    CHECK( get() == "ERROR: test3" );
    CHECK_NOTHROW( get() );
    CHECK( get() == "\ttest4" );
    CHECK( get() == "\t\t\ttest5" );
    CHECK( get() == "\t\ttest6" );
    CHECK_NOTHROW( get() );
    CHECK( msg.size() - msg.compare("test7 ") == std::string("test7 ").size() );

    // Close file
    log.close();
}


}
}