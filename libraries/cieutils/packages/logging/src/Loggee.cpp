// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "cieutils/packages/logging/inc/Loggee.hpp"

// --- STL Includes ---
#include <string>


namespace cie::utils {


Loggee::Loggee( Logger& r_logger,
                const std::string& r_instanceName ) :
    NamedObject( r_instanceName ),
    _logger( &r_logger ),
    _timerID( r_logger.startTimer() )
{
    CIE_BEGIN_EXCEPTION_TRACING

    r_logger.logElapsed( "[" + _name + "] Create at log time:", 0 );

    CIE_END_EXCEPTION_TRACING
}


Loggee::~Loggee()
{
    _logger->logElapsed( "[" + _name + "] Destroy, ran for", _timerID );
}


void Loggee::log(   const std::string& message,
                    size_t messageType  )
{
    std::string msg = "[" +  _name + "] " + message;

    switch(messageType)
    {
        case LOG_TYPE_MESSAGE:
            _logger->log( msg );
            break;

        case LOG_TYPE_WARNING:
            _logger->warn( msg );
            break;
        
        case LOG_TYPE_ERROR:
            _logger->error( msg );
            break;
        
        default:
            _logger->error( "Invalid message type!" );
            break;
    }
}


void Loggee::logID( const std::string& message,
                    size_t id,
                    size_t messageType  )
{
    CIE_BEGIN_EXCEPTION_TRACING

    log( message + " | ID_" + std::to_string(id), messageType );

    CIE_END_EXCEPTION_TRACING
}


size_t Loggee::tic()
{
    CIE_BEGIN_EXCEPTION_TRACING

    return _logger->startTimer();

    CIE_END_EXCEPTION_TRACING
}


void Loggee::toc(   const std::string& message,
                    size_t timerID,
                    bool reset )
{
    CIE_BEGIN_EXCEPTION_TRACING

    _logger->logElapsed( "[" + _name + "] " + message, timerID, reset );

    CIE_END_EXCEPTION_TRACING
}


void Loggee::toc(   size_t timerID,
                    bool reset )
{
    CIE_BEGIN_EXCEPTION_TRACING

    toc( "", timerID, reset );

    CIE_END_EXCEPTION_TRACING
}


void Loggee::separate()
{
    CIE_BEGIN_EXCEPTION_TRACING

    _logger->separate();

    CIE_END_EXCEPTION_TRACING
}


FileManager& Loggee::fileManager()
{
    CIE_BEGIN_EXCEPTION_TRACING

    return _logger->fileManager();

    CIE_END_EXCEPTION_TRACING
}


Logger& Loggee::logger()
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( _logger )
    return *_logger;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::utils