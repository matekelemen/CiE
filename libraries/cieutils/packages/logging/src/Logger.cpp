// --- Internal Includes ---
#include "cieutils/packages/logging/inc/Logger.hpp"
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
    _separator( "------------------------------------------------------" ),
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


void Logger::addStream( StreamPtr stream )
{
    auto it = std::find(    _streams.begin(),
                            _streams.end(),
                            stream );

    if (it == _streams.end())   
        _streams.push_back(stream);
}


void Logger::removeStream( StreamPtr stream )
{
    auto it = std::find(    _streams.begin(),
                            _streams.end(),
                            stream );

    if (it != _streams.end() && it!=_streams.begin())   
        _streams.erase(it);
}


void Logger::useConsole( bool use )
{
    _useConsole = use;
}


void Logger::forceFlush( bool use )
{
    _forceFlush = use;
}


void Logger::log( const std::string& message )
{
    // Get decorated message
    std::string msg = decorate(message);

    // Print to console if enabled
    if (_useConsole)
    {
        std::cout << msg;
        if (_forceFlush)
            std::flush(std::cout);
    }

    // Streams
    printToStreams( msg );
}


void Logger::warn( const std::string& message )
{
    log( "WARNING: " + message );
}


void Logger::error( const std::string& message )
{
    log( "ERROR: " + message );
    CIE_THROW( std::runtime_error, message );
}


void Logger::logDate( const std::string& message )
{
    log( message + " " + detail::getDate() );
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
    size_t t = (size_t)std::chrono::duration_cast<std::chrono::milliseconds>(detail::getTime() - _timeLog[slotID]).count();

    // Reset timer if requested
    if (reset)
        _timeLog[slotID] = _timeLog[0];

    return t;
}


void Logger::logElapsed(    const std::string& message,
                            size_t timeID,
                            bool reset )
{
    log( message + " " + std::to_string( elapsed(timeID, reset) ) + " [ms]" );
}


void Logger::separate()
{
    log( _separator );
}


void Logger::increaseIndent()
{
    _prefix += "\t";
}


void Logger::decreaseIndent()
{
    if (_prefix.back() == '\t')
        _prefix.pop_back();
}


void Logger::noIndent()
{
    while( _prefix.back() == '\t' )
        _prefix.pop_back();
}


FileManager& Logger::fileManager()
{
    return _manager;
}


void Logger::flush()
{
    std::flush( std::cout );

    for ( auto& stream : _streams )
        std::flush(*stream);
}


std::string Logger::decorate( const std::string& message )
{
    return _prefix + message + "\n";
}


void Logger::printToStreams( const std::string& message )
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
}





} // namespace cie::utils