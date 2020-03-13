#ifndef CIE_CIEUTILS_LOGGER_HPP
#define CIE_CIEUTILS_LOGGER_HPP

// --- Internal Includes ---
#include "../../output/inc/FileManager.hpp"
#include "../../output/inc/fileinfo.hpp"

// --- STD Includes ---
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>


// Define message types
namespace cie {

const size_t LOG_TYPE_MESSAGE   = 0;
const size_t LOG_TYPE_WARNING   = 1;
const size_t LOG_TYPE_ERROR     = 2;

}


namespace cie {
namespace utils {


// Clock definition
namespace detail
{
using Time = std::chrono::system_clock::time_point;

auto getTime = []() ->Time 
    { return std::chrono::high_resolution_clock::now(); };

auto getDate = []()
    {   auto t = std::chrono::system_clock::to_time_t(getTime());
        return std::string(std::ctime(&t)); };
}


// Type aliases
using Stream        = std::ostream;
using StreamPtr     = std::shared_ptr<Stream>;
struct Cout
{
    static Stream& cout;
};
Stream& Cout::cout = std::cout;


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

    void logDate();
    size_t startTimer();
    size_t elapsed( size_t timeID, 
                    bool reset=true );

    void separate();

    void increaseIndent();
    void decreaseIndent();
    void noIndent();

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