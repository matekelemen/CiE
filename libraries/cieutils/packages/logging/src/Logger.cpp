// --- Internal Includes ---
#include "../inc/Logger.hpp"

// --- STD Includes ---
#include <algorithm>


namespace cie {
namespace utils {


Logger::Logger( const std::string& fileName ) :
    _manager( detail::fileDirectory(fileName) ),
    _timeLog( {detail::getTime()} ),
    _prefix( "" ),
    _separator( "------------------------------------------------------" ),
    _useConsole( false ),
    _forceFlush( true )
{
    // Create log file
    addStream( StreamPtr(&_manager.newFile(fileName)) );

    // Log time
    log( "Log file created at " + detail::getDate() );
}


Logger::~Logger()
{
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
    log( "WARNING:\t" + message );
}


void Logger::error( const std::string& message )
{
    log( "ERROR:\t" + message );
}


void Logger::logDate()
{
    log( detail::getDate() );
}


size_t Logger::startTimer()
{
    // Find an idle slot
    size_t slotID = std::distance(  _timeLog.begin(),
                                    std::find(  _timeLog.begin()+1,
                                                _timeLog.end(),
                                                _timeLog[0]         ) ) + 1;
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
    size_t t = (size_t)std::chrono::duration_cast<std::chrono::milliseconds>(detail::getTime() - _timeLog[slotID]).count();
    if (reset)
        _timeLog[slotID] = _timeLog[0];

    return t;
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





}
}