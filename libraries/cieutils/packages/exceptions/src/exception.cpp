// --- Internal Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"

namespace cie {


Exception::Exception(   const String& r_location,
                        const String& r_message,
                        Size stackLevel ) :
    _stackLevel( stackLevel )
{
    _what =
        "(Exception stack " + std::to_string(_stackLevel) + ")\n"
        + r_location + "\n";

    if ( r_message != "" )
        _what += r_message + "\n";
}


Exception::Exception(   const String& r_location,
                        const String& r_message ) :
    Exception(  r_location,
                r_message,
                0 )
{
}


Exception::Exception(   const String& r_what,
                        Size stackLevel ) :
    _stackLevel( stackLevel ),
    _what( r_what )
{
}


Exception::Exception( const Exception& r_rhs ) :
    Exception(  r_rhs._what,
                r_rhs._stackLevel )
{
}

const char* Exception::what() const noexcept
{
    return _what.c_str();
}


Size Exception::stackLevel() const
{
    return _stackLevel;
}



NullPtrException::NullPtrException( const String& r_location,
                                    const String& r_message ) :
    Exception( r_location, "Attempt to dereference a nullptr:\n" + r_message )
{
}


AbstractCallException::AbstractCallException(   const String& r_location,
                                                const String& r_message ) :
    Exception( r_location, "Call to member function of abstact class:\n" + r_message )
{
}


NotImplementedException::NotImplementedException(   const String& r_location,
                                                    const String& r_message ) :
    Exception( r_location, "Call to not implemented function/class:\n" + r_message )
{
}


OutOfRangeException::OutOfRangeException(   const String& r_location,
                                            const String& r_message ) :
    Exception( r_location, "Index out-of-range or size mismatch:\n" + r_message )
{
}


DivisionByZeroException::DivisionByZeroException(   const String& r_location,
                                                    const String& r_message ) :
    Exception( r_location, "Division by zero exception:\n" + r_message )
{
}


GeometryException::GeometryException(   const String& r_location,
                                        const String& r_message ) :
    Exception( r_location, "Geometry exception\n" + r_message )
{
}


MemoryException::MemoryException( const String& r_location,
                                  const String& r_message ) :
    Exception( r_location, "Memory exception\n" + r_message )
{
}


} // namespace cie