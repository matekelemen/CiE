// --- Internal Includes ---
#include "cieutils/packages/logging/inc/Loggee.hpp"

// --- STL Includes ---
#include <string>


namespace cie::utils {


Loggee::Loggee( Logger& logger,
                const std::string& instanceName ) :
    _logger( &logger ),
    _name( instanceName ),
    _timerID( logger.startTimer() )
{
    logger.logElapsed( "[" + _name + "] Create at log time:", 0 );
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
    log( message + " | id_" + std::to_string(id), messageType );
}


size_t Loggee::tic()
{
    return _logger->startTimer();
}


void Loggee::toc(   const std::string& message,
                    size_t timerID,
                    bool reset )
{
    _logger->logElapsed( "[" + _name + "] " + message, timerID, reset );
}


void Loggee::toc(   size_t timerID,
                    bool reset )
{
    toc( "", timerID, reset );
}


void Loggee::separate()
{
    _logger->separate();
}


FileManager& Loggee::fileManager()
{
    return _logger->fileManager();
}


Logger& Loggee::logger()
{
    return *_logger;
}


} // namespace cie::utils