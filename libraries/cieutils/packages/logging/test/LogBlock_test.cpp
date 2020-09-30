// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "cieutils/packages/logging/inc/LogBlock.hpp"
#include "cmake_variables.hpp"


namespace cie::utils {


TEST_CASE( "LogBlock", "[logging]" )
{
    std::string loggerTestDir = TEST_OUTPUT_PATH;

    Logger logger( loggerTestDir + "/LogBlock_test.txt" );
    logger.useConsole(true);
    
    for (Size i=0; i<10; ++i)
        if ( i % 2 == 0 )
            LogBlock block( "iteration " + std::to_string(i), logger );
}


} // namespace cie::utils