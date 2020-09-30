// --- Internal Includes ---
#include "cieutils/packages/logging/inc/LogBlock.hpp"


namespace cie::utils {


LogBlock::LogBlock( const std::string& r_name,
                    Logger& r_logger ) :
    _name( r_name ),
    _timerID( 0 /*r_logger.startTimer()*/ ),
    _r_logger( r_logger )
{
    r_logger.separate().increaseIndent().log( "> " + _name ).flush();
}


LogBlock::~LogBlock()
{
    logElapsed( "< " + _name, false );
    _r_logger.decreaseIndent().separate().flush();
}


inline LogBlock& LogBlock::log( const std::string& r_message )
{
    _r_logger.log( r_message );
    return *this;
}


inline LogBlock& LogBlock::warn( const std::string& r_message )
{
    _r_logger.warn( r_message );
    return *this;
}


inline LogBlock& LogBlock::error( const std::string& r_message )
{
    _r_logger.error( r_message );
    return *this;
}


inline LogBlock& LogBlock::logElapsed(  const std::string& r_message,
                                        bool reset )
{
    _r_logger.logElapsed(   r_message,
                            _timerID,
                            reset );
    return *this;
}


} // namespace cie::utils