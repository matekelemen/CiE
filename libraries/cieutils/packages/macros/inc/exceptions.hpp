#ifndef CIE_UTILS_EXCEPTION_MACROS_HPP
#define CIE_UTILS_EXCEPTION_MACROS_HPP

// --- Internal Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/macros/inc/detail.hpp"

// --- STL Includes ---
#include <sstream>
#include <concepts>


/* --- Define exception concepts --- */
namespace cie::concepts {

template <class T>
concept CIEException
= std::derived_from<T,cie::Exception>;

template <class T>
concept STLException
= std::derived_from<T,std::exception>
        && (!CIEException<T>);

} // namespace concepts

/* --- Define exception factories --- */
namespace cie::detail {

template <concepts::CIEException ExceptionType>
ExceptionType makeException( const String& r_location,
                             const String& r_message,
                             const String& r_exceptionName )
{ return ExceptionType( r_location, r_message ); }

template <concepts::STLException ExceptionType>
Exception makeException(    const String& r_location,
                            const String& r_message,
                            const String& r_exceptionName )
{
    return Exception( r_location,
                      r_exceptionName + ": " + r_message );
}

} // namespace cie::detail



/* Define exception-related macros */

#define CIE_THROW(ExceptionType,message)                                    \
    {                                                                       \
        throw cie::detail::makeException<ExceptionType>( CIE_CODE_LOCATION, \
                                                         message,           \
                                                         #ExceptionType );  \
    }


#ifdef CIE_ENABLE_EXCEPTION_TRACING

    #define CIE_BEGIN_EXCEPTION_TRACING                                     \
        try {

    #define CIE_END_EXCEPTION_TRACING                                       \
        }                                                                   \
        catch ( const cie::Exception& EXCEPTION )                           \
        {                                                                   \
            throw cie::Exception(                                           \
                CIE_CODE_LOCATION,                                          \
                EXCEPTION.what(),                                           \
                EXCEPTION.stackLevel() + 1                                  \
            );                                                              \
        }                                                                   \
        catch ( const std::exception& EXCEPTION )                           \
        {                                                                   \
            throw cie::detail::makeException<cie::Exception>(               \
                CIE_CODE_LOCATION,                                          \
                EXCEPTION.what(),                                           \
                "std::exception"                                            \
            );                                                              \
        }                                                                   \
        catch ( ... )                                                       \
        {                                                                   \
            CIE_THROW( cie::Exception, "Unknown exception" )                \
        }

#else

    #define CIE_BEGIN_EXCEPTION_TRACING

    #define CIE_END_EXCEPTION_TRACING

#endif // CIE_ENABLE_EXCEPTION_TRACING


#endif