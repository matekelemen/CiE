#ifndef CIE_CIEUTILS_EXCEPTIONS_HPP
#define CIE_CIEUTILS_EXCEPTIONS_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <stdexcept>
#include <string>
#include <deque>
#include <memory>

namespace cie {


struct Exception;
using ExceptionPtr = std::shared_ptr<Exception>;



struct Exception : public std::exception
{
public:
    Exception() = delete;

    Exception(  const String& location,
                const String& message,
                Size stackLevel );

    Exception(  const String& location,
                const String& message );

    Exception(  const String& r_what,
                Size stackLevel );

    Exception( const Exception& r_rhs );
    
    const char* what() const noexcept override;

    Size stackLevel() const;

private:
    const Size      _stackLevel;
    String          _what;
};


struct NullPtrException : public Exception
{
    NullPtrException(   const String& r_location,
                        const String& r_message );
};


struct AbstractCallException : public Exception
{
    AbstractCallException(  const String& r_location,
                            const String& r_message );
};


struct NotImplementedException : public Exception
{
    NotImplementedException(    const String& r_location,
                                const String& r_message );
};


struct OutOfRangeException : public Exception
{
    OutOfRangeException(    const String& r_location,
                            const String& r_message );
};


struct DivisionByZeroException : public Exception
{
    DivisionByZeroException(    const String& r_location,
                                const String& r_message );
};


struct GeometryException : public Exception
{
    GeometryException(  const String& r_location,
                        const String& r_message );
};


} // namespace cie


#endif