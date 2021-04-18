#ifndef CIE_UTILS_LOGGING_LOGGER_SINGLETON_HPP
#define CIE_UTILS_LOGGING_LOGGER_SINGLETON_HPP

// --- Internal Includes ---
#include "cieutils/packages/logging/inc/Logger.hpp"


namespace cie::utils {


class LoggerSingleton
{
public:
    static Logger& get();

    static LoggerPtr getPtr();

private:
    static LoggerPtr _p_logger;
};


} // namespace cie::utils


#endif