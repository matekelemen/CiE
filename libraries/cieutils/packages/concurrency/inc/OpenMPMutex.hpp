#ifndef CIE_UTILS_CONCURRENCY_OPENMP_MUTEX_HPP
#define CIE_UTILS_CONCURRENCY_OPENMP_MUTEX_HPP

// --- Internal Includes ---
#include "cieutils/packages/concurrency/inc/AbsMutex.hpp"

// --- External Includes ---
#include <omp.h>


namespace cie::mp {


class OpenMPMutex final : public AbsMutex
{
public:
    using lock_type = omp_lock_t;

public:
    OpenMPMutex();
    ~OpenMPMutex();

protected:
    void lock_impl() override;
    void unlock_impl() override;

protected:
    int         _threadID;
    lock_type   _lock;
};


} // namespace cie::mp


#endif