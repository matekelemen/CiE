// --- Internal Includes ---
#include "cieutils/packages/logging/inc/LogBlock.hpp"


namespace cie::utils {


LogBlock::LogBlock( const std::string& r_name,
                    Logger& r_logger ) :
    _name( r_name ),
    _timerID( r_logger.startTimer() ),
    _r_logger( r_logger )
{
    r_logger.separate().log( "> " + _name ).increaseIndent().flush();
}


LogBlock::~LogBlock()
{
    _r_logger.decreaseIndent();
    logElapsed( "< " + _name + " |", false );
    _r_logger.separate().flush();
}


} // namespace cie::utils