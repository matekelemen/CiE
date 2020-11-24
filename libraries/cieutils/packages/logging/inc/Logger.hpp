#ifndef CIE_CIEUTILS_LOGGER_HPP
#define CIE_CIEUTILS_LOGGER_HPP

// --- Internal Includes ---
#include "cieutils/packages/output/inc/FileManager.hpp"
#include "cieutils/packages/output/inc/fileinfo.hpp"

#include "cieutils/packages/concepts/inc/streamable.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <fstream>
#include <iostream>
#include <deque>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>
#include <filesystem>


namespace cie::utils {


// Clock definition
namespace detail
{
using Time = std::chrono::steady_clock::time_point;

Time getTime();

std::string getDate();
} // namespace detail


// Type aliases
using Stream        = std::ostream;
using StreamPtr     = std::shared_ptr<Stream>;


class LogBlock;


class Logger
{
public:
    friend class LogBlock;

public:
    Logger() = delete;
    Logger( const std::filesystem::path& r_filePath,
            bool useConsole = false );
    Logger( const Logger& copy ) = delete;
    ~Logger();

    LogBlock newBlock( const std::string& r_name );

    Logger& addStream( StreamPtr stream );
    Logger& removeStream( StreamPtr stream );
    Logger& useConsole( bool use );
    Logger& forceFlush( bool use );

    Logger& log( const std::string& message );

    template <class ...Args>
    Logger& logs( Args&&... args );

    Logger& warn( const std::string& message );

    template <class ExceptionType = std::runtime_error>
    Logger& error( const std::string& r_message );

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
    bool                        _useConsole;
    bool                        _forceFlush;
};


using LoggerPtr = std::shared_ptr<Logger>;


} // namespace cie::utils



/* --- LOG STREAM OVERLOADS --- */

namespace cie::concepts {

template <class T>
concept Log
= requires ( T& r_instance, const std::string& r_message )
{
    { r_instance.log( r_message ) };
};

} // namespace cie::concepts

namespace cie::utils {


template <concepts::Log LogType>
LogType& operator<<( LogType& r_log,
                     const std::string& r_message );


template <concepts::Log LogType, class ContainerType>
requires concepts::STLContainer<ContainerType>
         && concepts::StringStreamable<typename ContainerType::value_type>
LogType& operator<<( LogType& r_log,
                     const ContainerType& r_messageContainer );


template <concepts::Log LogType, concepts::StringStreamable MessageType>
LogType& operator<<( LogType& r_log,
                     const MessageType& r_message );


} // namespace cie::utils


#include "cieutils/packages/logging/impl/Logger_impl.hpp"

#endif