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
    Exception( "Index out-of-range or size mismatch exception\nin function: " + message )
{
}


DivisionByZeroException::DivisionByZeroException( const String& message ) :
    Exception( "Division by zero exception\nin function: " + message )
{
}


GeometryException::GeometryException( const String& message, const String& functionName ) :
    Exception( "Geometry exception\nin function " + functionName + "\n" + message )
{
}


}