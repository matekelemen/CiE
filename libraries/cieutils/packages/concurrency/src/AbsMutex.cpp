// --- Internal Includes ---
#include "cieutils/packages/concurrency/inc/AbsMutex.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::mp {


AbsMutex::AbsMutex() :
    _isLocked( false )
{
}


void AbsMutex::lock()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->lock_impl();
    this->_isLocked = true;

    CIE_END_EXCEPTION_TRACING
}


void AbsMutex::unlock()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->unlock_impl();
    this->_isLocked = false;

    CIE_END_EXCEPTION_TRACING
}


bool AbsMutex::isLocked() const
{
    return this->_isLocked;
}


} // namespace cie::mp