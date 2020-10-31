#ifndef CIE_UTILS_MACROS_TESTING_HPP
#define CIE_UTILS_MACROS_TESTING_HPP

// --- Internal Includes ---
#include "cieutils/packages/logging/inc/LogBlock.hpp"
#include "cieutils/packages/logging/inc/Logger.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <string>


namespace cie::utils {


class STATIC_LOG_BLOCK : public LogBlock
{
public:
    STATIC_LOG_BLOCK( const std::string& name ) : LogBlock( name, _logger ) 
    { _logger.useConsole(true); }
    static Logger _logger;
};



} // namespace cie::utils


#define CIE_TESTRUNNER_INIT( sourceName ) \
    cie::utils::Logger cie::utils::STATIC_LOG_BLOCK::_logger( cie::TEST_OUTPUT_PATH + "/test_" + sourceName + "_log.txt", true );

#define CIE_TEST_CASE_INIT( testCaseName ) \
    cie::utils::STATIC_LOG_BLOCK LOG_BLOCK( testCaseName );

#endif