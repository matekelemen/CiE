// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "cieutils/packages/logging/inc/Logger.hpp"
#include "cieutils/packages/logging/inc/LogBlock.hpp"
#include "cieutils/packages//macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::utils {


namespace detail {

Time getTime()
{
    CIE_BEGIN_EXCEPTION_TRACING

    return std::chrono::steady_clock::now();

    CIE_END_EXCEPTION_TRACING
}

std::string getDate()
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return std::string(std::ctime(&t));

    CIE_END_EXCEPTION_TRACING
}

} // namespace detail


Logger::Logger( const std::filesystem::path& r_filePath,
                bool useConsole ) :
    _manager( detail::fileDirectory( r_filePath ) ),
    _timeLog( {detail::getTime()} ),
    _prefix( "" ),
    _useConsole( useConsole ),
    _forceFlush( true )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Create log file
    File& logFile = _manager.newFile( r_filePath );
    addStream( _manager.filePtr( logFile ) );

    // Log time
    logDate( "Log file created on" );

    CIE_END_EXCEPTION_TRACING
}


Logger::~Logger()
{
    noIndent();
    logElapsed( "\nLogger ran for", 0, false );
    logDate( "Log file closed on" );
    flush();
}


LogBlock Logger::newBlock( const std::string& r_name )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return LogBlock( r_name, *this );

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::addStream( StreamPtr stream )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it = std::find(    _streams.begin(),
                            _streams.end(),
                            stream );

    if (it == _streams.end())   
        _streams.push_back(stream);

    return *this;

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::removeStream( StreamPtr stream )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it = std::find(    _streams.begin(),
                            _streams.end(),
                            stream );

    if (it != _streams.end() && it!=_streams.begin())   
        _streams.erase(it);

    return *this;

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::useConsole( bool use )
{
    _useConsole = use;
    return *this;
}


Logger& Logger::forceFlush( bool use )
{
    _forceFlush = use;
    return *this;
}


Logger& Logger::log( const std::string& message )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return log( message, true );

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::warn( const std::string& message )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return log( "WARNING: " + message );

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::logDate( const std::string& message )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return log( message + " " + detail::getDate() );

    CIE_END_EXCEPTION_TRACING
}


size_t Logger::startTimer()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Find an idle slot
    size_t slotID = std::distance(  _timeLog.begin(),
                                    std::find(  _timeLog.begin()+1,
                                                _timeLog.end(),
                                                _timeLog[0]         ) );
    if (slotID == _timeLog.size())
    {
        _timeLog.push_back( _timeLog[0] );
        slotID = _timeLog.size() - 1;
    }

    // Start timer
    _timeLog[slotID] = detail::getTime();
    return slotID;

    CIE_END_EXCEPTION_TRACING
}


size_t Logger::elapsed( size_t slotID, bool reset )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Check if valid slot
    if ( slotID >= _timeLog.size() )
        error( "Invalid timerID " + std::to_string(slotID) );

    // Compute elapsed time
    size_t t = (size_t)std::chrono::duration_cast<std::chrono::microseconds>(detail::getTime() - _timeLog[slotID]).count();

    // Reset timer if requested
    if (reset)
        _timeLog[slotID] = _timeLog[0];

    return t;

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::logElapsed( const std::string& message,
                            size_t timeID,
                            bool reset )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto dt = elapsed( timeID, reset );
    std::string unit = " [us] ";

    if ( dt > 100000 )
    {
        dt      /= 1000;
        unit    = " [ms] ";

        if ( dt > 100000 )
        {
            dt      /= 1000;
            unit    = " [s] ";

            if ( dt > 6000 )
            {
                dt      /= 60;
                unit    = " [min] ";
            }
        }
    }

    return log( message + " " + std::to_string( dt ) + unit );

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::separate()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // TODO: separator behaviour is hard-coded
    const char separatorCharacter   = '-';
    const Size lineWidth            = 60;

    Size numberOfSeparators         = 0;
    if ( lineWidth > _prefix.size() )
        numberOfSeparators = lineWidth - _prefix.size();

    return log( std::string( numberOfSeparators, separatorCharacter ),
                true );

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::increaseIndent()
{
    _prefix += "    ";
    return *this;
}


Logger& Logger::decreaseIndent()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( _prefix.size() >= 4 )
        _prefix.resize( _prefix.size() - 4 );
    else
        _prefix.clear();
    return *this;

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::noIndent()
{
    CIE_BEGIN_EXCEPTION_TRACING

    while( _prefix.back() == '\t' )
        _prefix.pop_back();
    return *this;
    
    CIE_END_EXCEPTION_TRACING
}


FileManager& Logger::fileManager()
{
    return _manager;
}


Logger& Logger::log(    const std::string& message,
                        bool printPrefix )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Get decorated message
    std::string msg = decorate( message, printPrefix );

    // Print to console if enabled
    if (_useConsole)
    {
        std::cout << msg;
        if (_forceFlush)
            std::flush(std::cout);
    }

    // Streams
    return printToStreams( msg );

    CIE_END_EXCEPTION_TRACING
}


Logger& Logger::flush()
{
    CIE_BEGIN_EXCEPTION_TRACING

    std::flush( std::cout );

    for ( auto& stream : _streams )
        std::flush(*stream);

    return *this;

    CIE_END_EXCEPTION_TRACING
}


std::string Logger::decorate(   const std::string& message,
                                bool prefix )
{
    return (prefix ? _prefix + message : message) + "\n"; 
}


Logger& Logger::printToStreams( const std::string& message )
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( auto& stream : _streams )
    {
        try
        {
            *stream << message;
            if (_forceFlush)
                std::flush(*stream);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    return *this;

    CIE_END_EXCEPTION_TRACING
}



Logger& operator<<( Logger& r_logger, const std::string& r_message )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return r_logger.log( r_message );

    CIE_END_EXCEPTION_TRACING
}





} // namespace cie::utils