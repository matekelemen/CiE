#ifndef CIE_CIEUTILS_EXCEPTIONS_HPP
#define CIE_CIEUTILS_EXCEPTIONS_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <stdexcept>
#include <string>

namespace cie {


struct Exception : public std::exception
{
public:
    Exception() = delete;
    Exception( const String& message );
    const char* what() const noexcept override;

protected:
    String _what;
};


struct NullPtrException : public Exception
{
    NullPtrException( const String& message );
};


struct AbstractCallException : public Exception
{
    AbstractCallException( const String& message );
};


struct NotImplementedException : public Exception
{
    NotImplementedException( const String& message );
};


struct OutOfRangeException : public Exception
{
    OutOfRangeException( const String& message );
};


struct DivisionByZeroException : public Exception
{
    DivisionByZeroException( const String& message );
};


struct GeometryException : public Exception
{
    GeometryException( const String& message, const String& functionName );
};


}


#endif