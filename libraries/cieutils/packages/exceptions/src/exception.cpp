// --- Internal Includes ---
#include "../inc/exception.hpp"

namespace cie {


Exception::Exception( const String& message ) :
    _what(message)
{
}


const char* Exception::what() const noexcept
{
    return _what.c_str();
}


AbstractCallException::AbstractCallException( const String& functionName ) :
    Exception( "Call to member function of abstact class:\n\t" + functionName )
{
}


OutOfRangeException::OutOfRangeException( const String& message ) :
    Exception( "Out-of-range or size mismatch:\n\t" + message )
{
}


}