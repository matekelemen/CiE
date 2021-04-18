// --- Internal Includes ---
#include "cieutils/packages/logging/inc/LoggerSingleton.hpp"
#include "cmake_variables.hpp"


namespace cie::utils {


LoggerPtr LoggerSingleton::_p_logger( new Logger(OUTPUT_PATH / "cie.log") );


Logger& LoggerSingleton::get()
{
    return *LoggerSingleton::_p_logger;
}


LoggerPtr LoggerSingleton::getPtr()
{
    return LoggerSingleton::_p_logger;
}


} // namespace cie::utils