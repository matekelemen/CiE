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
    return std::chrono::steady_clock::now();
}

std::string getDate()
{
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return std::string(std::ctime(&t));
}

} // namespace detail


Logger::Logger( const std::string& fileName ) :
    _manager( detail::fileDirectory(fileName) ),
    _timeLog( {detail::getTime()} ),
    _prefix( "" ),
    _useConsole( false ),
    _forceFlush( true )
{
    // Create log file
    File& logFile = _manager.newFile(fileName);
    addStream( _manager.filePtr(logFile) );

    // Log time
    logDate( "Log file created on" );
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
    return LogBlock( r_name, *this );
}


Logger& Logger::addStream( StreamPtr stream )
{
    auto it = std::find(    _streams.begin(),
                            _streams.end(),
                            stream );

    if (it == _streams.end())   
        _streams.push_back(stream);

    return *this;
}


Logger& Logger::removeStream( StreamPtr stream )
{
    auto it = std::find(    _streams.begin(),
                            _streams.end(),
                            stream );

    if (it != _streams.end() && it!=_streams.begin())   
        _streams.erase(it);

    return *this;
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
    return log( message, true );
}


Logger& Logger::warn( const std::string& message )
{
    return log( "WARNING: " + message );
}


Logger& Logger::error( const std::string& message )
{
    log( "ERROR: " + message );
    CIE_THROW( std::runtime_error, message );
    return *this;
}


Logger& Logger::logDate( const std::string& message )
{
    return log( message + " " + detail::getDate() );
}


size_t Logger::startTimer()
{
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
}


size_t Logger::elapsed( size_t slotID, bool reset )
{

    // Check if valid slot
    if ( slotID >= _timeLog.size() )
        error( "Invalid timerID " + std::to_string(slotID) );

    // Compute elapsed time
    size_t t = (size_t)std::chrono::duration_cast<std::chrono::microseconds>(detail::getTime() - _timeLog[slotID]).count();

    // Reset timer if requested
    if (reset)
        _timeLog[slotID] = _timeLog[0];

    return t;
}


Logger& Logger::logElapsed( const std::string& message,
                            size_t timeID,
                            bool reset )
{
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
}


Logger& Logger::separate()
{
    // TODO: separator behaviour is hard-coded
    const char separatorCharacter   = '-';
    const Size lineWidth            = 60;

    Size numberOfSeparators         = 0;
    if ( lineWidth > _prefix.size() )
        numberOfSeparators = lineWidth - _prefix.size();

    return log( std::string( numberOfSeparators, separatorCharacter ),
                true );
}


Logger& Logger::increaseIndent()
{
    _prefix += "    ";
    return *this;
}


Logger& Logger::decreaseIndent()
{
    if ( _prefix.size() >= 4 )
        _prefix.resize( _prefix.size() - 4 );
    else
        _prefix.clear();
    return *this;
}


Logger& Logger::noIndent()
{
    while( _prefix.back() == '\t' )
        _prefix.pop_back();
    return *this;
}


FileManager& Logger::fileManager()
{
    return _manager;
}


Logger& Logger::log(    const std::string& message,
                        bool printPrefix )
{
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
}


Logger& Logger::flush()
{
    std::flush( std::cout );

    for ( auto& stream : _streams )
        std::flush(*stream);

    return *this;
}


std::string Logger::decorate(   const std::string& message,
                                bool prefix )
{
    return (prefix ? _prefix + message : message) + "\n"; 
}


Logger& Logger::printToStreams( const std::string& message )
{
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
}



Logger& operator<<( Logger& r_logger, const std::string& r_message )
{
    return r_logger.log( r_message );
}





} // namespace cie::utils