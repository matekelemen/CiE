#ifndef CIE_CIEUTILS_EXCEPTIONS_HPP
#define CIE_CIEUTILS_EXCEPTIONS_HPP

// --- Utility Includes ---
#include <cieutils/types.hpp>

// --- STD Includes ---
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


struct AbstractCallException : public Exception
{
    AbstractCallException( const String& functionName );
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