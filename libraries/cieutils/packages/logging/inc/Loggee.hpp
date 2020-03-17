#ifndef CIEUTILS_LOGGEE_HPP
#define CIEUTILS_LOGGEE_HPP

// --- Internal Includes ---
#include "Logger.hpp"

// --- STD Includes ---
#include <string>


// Logger message flags
namespace cie {
const size_t LOG_TYPE_MESSAGE   = 0;
const size_t LOG_TYPE_WARNING   = 1;
const size_t LOG_TYPE_ERROR     = 2;
}


namespace cie {
namespace utils {


class Loggee
{
public:
    Loggee( Logger& logger, 
            const std::string& instanceName );
    ~Loggee();

    void log(   const std::string& message,
                size_t messageType = LOG_TYPE_MESSAGE );
    void logID( const std::string& message,
                size_t id,
                size_t messageType = LOG_TYPE_MESSAGE );

    [[nodiscard]] size_t tic();
    void toc(   size_t timerID,
                bool reset = true );
    void toc(   const std::string& message,
                size_t timerID,
                bool reset = true );

    void separate();

    FileManager& fileManager();

protected:
    Logger& logger();

private:
    Logger*         _logger;
    std::string     _name;
    size_t          _timerID;
};


}
}


#endif