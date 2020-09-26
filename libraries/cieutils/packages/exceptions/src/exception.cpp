// --- Internal Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"

namespace cie {


Exception::Exception( const String& message ) :
    _what(message)
{
}


const char* Exception::what() const noexcept
{
    return _what.c_str();
}


NullPtrException::NullPtrException( const String& message ) :
    Exception( "Attempt to dereference a nullptr:\n" + message )
{
}


AbstractCallException::AbstractCallException( const String& message ) :
    Exception( "Call to member function of abstact class:\n" + message )
{
}


NotImplementedException::NotImplementedException( const String& message ) :
    Exception( "Call to not implemented function/class:\n" + message )
{
}


OutOfRangeException::OutOfRangeException( const String& message ) :
    Exception( "Index out-of-range or size mismatch:\n" + message )
{
}


DivisionByZeroException::DivisionByZeroException( const String& message ) :
    Exception( "Division by zero:\n" + message )
{
}


GeometryException::GeometryException( const String& message, const String& functionName ) :
    Exception( "Geometry exception\nin function " + functionName + "\n" + message )
{
}


}