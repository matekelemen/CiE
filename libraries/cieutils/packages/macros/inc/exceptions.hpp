#ifndef CIE_UTILS_EXCEPTION_MACROS_HPP
#define CIE_UTILS_EXCEPTION_MACROS_HPP

// --- Internal Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/macros/inc/detail.hpp"

// --- STL Includes ---
#include <sstream>


#define CIE_THROW(ExceptionType,message)                                    \
    {                                                                       \
        std::stringstream msg;                                              \
        msg << "Exception of type " << #ExceptionType << " thrown\n";       \
        msg << CIE_CODE_LOCATION << "\n";                                   \
        msg << "with message: " << message;                                 \
        throw ExceptionType( msg.str() );                                   \
    }


#ifdef CIE_ENABLE_EXCEPTION_TRACING

    #define CIE_BEGIN_EXCEPTION_TRACING                                     \
        try {

    #define CIE_END_EXCEPTION_TRACING                                       \
        catch ( const cie::Exception& EXCEPTION )                           \
        {                                                                   \
            throw EXCEPTION;                                                \
        }                                                                   \
        catch ( const std::exception& EXCEPTION )                           \
        {                                                                   \
            throw EXCEPTION;                                                \
        }                                                                   \
        catch ( ... )                                                       \
        {                                                                   \
            throw cie::Exception( "Unknown exception" );                    \
        }

#else

    #define CIE_BEGIN_EXCEPTION_TRACING

    #define CIE_END_EXCEPTION_TRACING

#endif // CIE_ENABLE_EXCEPTION_TRACING


#endif