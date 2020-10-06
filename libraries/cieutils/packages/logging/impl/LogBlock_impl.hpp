#ifndef CIE_UTILS_LOGGING_LOG_BLOCK_IMPL_HPP
#define CIE_UTILS_LOGGING_LOG_BLOCK_IMPL_HPP

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::utils {


inline LogBlock& LogBlock::log( const std::string& r_message )
{
    CIE_BEGIN_EXCEPTION_TRACING

    _r_logger.log( r_message );
    return *this;

    CIE_END_EXCEPTION_TRACING
}


inline LogBlock& LogBlock::warn( const std::string& r_message )
{
    CIE_BEGIN_EXCEPTION_TRACING

    _r_logger.warn( r_message );
    return *this;

    CIE_END_EXCEPTION_TRACING
}


inline LogBlock& LogBlock::error( const std::string& r_message )
{
    CIE_BEGIN_EXCEPTION_TRACING

    _r_logger.error( r_message );
    return *this;

    CIE_END_EXCEPTION_TRACING
}


inline LogBlock& LogBlock::logElapsed(  const std::string& r_message,
                                        bool reset )
{
    CIE_BEGIN_EXCEPTION_TRACING

    _r_logger.logElapsed(   r_message,
                            _timerID,
                            reset );
    return *this;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::utils


#endif