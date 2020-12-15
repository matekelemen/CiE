// --- Internal Includes ---
#include "cieutils/packages/concurrency/inc/OpenMPMutex.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

namespace cie::mp {


OpenMPMutex::OpenMPMutex() :
    _threadID(-1)
{
    omp_init_lock(&_lock);
}


OpenMPMutex::~OpenMPMutex()
{
    omp_unset_lock( &_lock ); // <-- force unset before destroying the lock
    omp_destroy_lock( &_lock );
}


void OpenMPMutex::lock_impl()
{
    omp_set_lock( &_lock );
    _threadID = omp_get_thread_num();
}


void OpenMPMutex::unlock_impl()
{
    if ( omp_get_thread_num() != _threadID )
    {
        CIE_THROW(
            std::runtime_error,
            "Thread " + std::to_string(omp_get_thread_num()) + " attempted to unlock OpenMPMutex locked by thread " + std::to_string(_threadID)
            )
    }

    omp_unset_lock( &_lock );
}


} // namespace cie::mp