// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "cieutils/packages/logging/inc/Logger.hpp"
#include "cieutils/packages/macros/inc/testing.hpp"
#include "cmake_variables.hpp"


namespace cie::utils {


TEST_CASE( "Logger", "[logger]" )
{
    CIE_TEST_CASE_INIT( "Logger" )

    std::filesystem::path loggerTestDir = TEST_OUTPUT_PATH;

    {
        // Construct logger
        Logger logger( loggerTestDir / "testLog.txt",
                       true );

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
}


}