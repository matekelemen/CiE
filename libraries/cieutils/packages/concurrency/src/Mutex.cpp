// --- Internal Includes ---
#include "cieutils/packages/concurrency/inc/Mutex.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

namespace cie::mp {


Mutex::Mutex() :
    _isLocked(false),
    _threadID(-1)
{
    omp_init_lock(&_lock);
}


Mutex::~Mutex()
{
    forceUnlock();
    omp_destroy_lock( &_lock );
}


void Mutex::lock()
{
    omp_set_lock( &_lock );
    _isLocked = true;
    _threadID = omp_get_thread_num();
}


void Mutex::unlock()
{
    if ( omp_get_thread_num() != _threadID )
    {
        CIE_THROW(
            std::runtime_error,
            "Thread " + std::to_string(omp_get_thread_num()) + " attempted to unlock Mutex locked by thread " + std::to_string(_threadID)
            )
    }

    omp_unset_lock( &_lock );
    _isLocked = false;
}


bool Mutex::isLocked() const
{
    return _isLocked;
}


void Mutex::forceUnlock()
{
    omp_unset_lock( &_lock );
    _isLocked = false;
}


} // namespace cie::mp