#ifndef CIE_UTILS_LOGGER_IMPL_HPP
#define CIE_UTILS_LOGGER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::utils {


template <class ExceptionType>
Logger& Logger::error( const std::string& r_message )
{
    CIE_BEGIN_EXCEPTION_TRACING

    log( "ERROR: " + r_message );
    CIE_THROW( ExceptionType, r_message );
    return *this;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::utils


#endif