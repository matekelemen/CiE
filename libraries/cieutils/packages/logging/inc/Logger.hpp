#ifndef CIE_CIEUTILS_LOGGER_HPP
#define CIE_CIEUTILS_LOGGER_HPP

// --- Internal Includes ---
#include "../../output/inc/FileManager.hpp"
#include "../../output/inc/fileinfo.hpp"

// --- STL Includes ---
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>


namespace cie {
namespace utils {


// Clock definition
namespace detail
{
using Time = std::chrono::steady_clock::time_point;

Time getTime();

std::string getDate();
}


// Type aliases
using Stream        = std::ostream;
using StreamPtr     = std::shared_ptr<Stream>;


class Logger
{
public:
    Logger() = delete;
    Logger( const std::string& fileName );
    Logger( const Logger& copy ) = delete;
    ~Logger();

    void addStream( StreamPtr stream );
    void removeStream( StreamPtr stream );
    void useConsole( bool use );
    void forceFlush( bool use );

    void log(   const std::string& message );
    void warn(  const std::string& message );
    void error( const std::string& message );

    void logDate( const std::string& message );
    [[nodiscard]] size_t startTimer();
    size_t elapsed( size_t timeID, 
                    bool reset=true );
    void logElapsed(    const std::string& message, 
                        size_t timeID,
                        bool reset=true );

    void separate();

    void increaseIndent();
    void decreaseIndent();
    void noIndent();

    FileManager& fileManager();

protected:
    void flush();
    std::string decorate( const std::string& message );
    void printToStreams( const std::string& message );

    FileManager                 _manager;
    std::vector<StreamPtr>      _streams;
    std::vector<detail::Time>   _timeLog;

    std::string                 _prefix;
    std::string                 _separator;
    bool                        _useConsole;
    bool                        _forceFlush;
};


}
}

#endif