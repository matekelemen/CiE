#ifndef CIE_CIEUTILS_LOGGER_HPP
#define CIE_CIEUTILS_LOGGER_HPP

// --- Internal Includes ---
#include "cieutils/packages/output/inc/FileManager.hpp"
#include "cieutils/packages/output/inc/fileinfo.hpp"

// --- STL Includes ---
#include <fstream>
#include <iostream>
#include <deque>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>


namespace cie::utils {


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
    friend class LogBlock;

public:
    Logger() = delete;
    Logger( const std::string& fileName );
    Logger( const Logger& copy ) = delete;
    ~Logger();

    Logger& addStream( StreamPtr stream );
    Logger& removeStream( StreamPtr stream );
    Logger& useConsole( bool use );
    Logger& forceFlush( bool use );

    Logger& log(   const std::string& message );
    Logger& warn(  const std::string& message );
    Logger& error( const std::string& message );

    Logger& logDate( const std::string& message );
    [[nodiscard]] size_t startTimer();
    size_t elapsed( size_t timeID, 
                    bool reset=true );
    Logger& logElapsed(    const std::string& message, 
                        size_t timeID,
                        bool reset=true );

    Logger& separate();

    Logger& increaseIndent();
    Logger& decreaseIndent();
    Logger& noIndent();

    FileManager& fileManager();

protected:
    Logger& log(    const std::string& r_message,
                    bool printPrefix );

    Logger& flush();
    std::string decorate(   const std::string& message,
                            bool prefix = true );
    Logger& printToStreams( const std::string& message );

    FileManager                 _manager;
    std::deque<StreamPtr>       _streams;
    std::deque<detail::Time>    _timeLog;

    std::string                 _prefix;
    std::string                 _separator;
    bool                        _useConsole;
    bool                        _forceFlush;
};


} // namespace cie::utils

#endif