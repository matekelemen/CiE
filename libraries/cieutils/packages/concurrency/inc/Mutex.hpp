#ifndef CIE_UTILS_CONCURRENCY_MUTEX_HPP
#define CIE_UTILS_CONCURRENCY_MUTEX_HPP

// --- External Includes ---
#include <omp.h>


namespace cie::mp {


class Mutex
{
public:
    using lock_type = omp_lock_t;

public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    bool isLocked() const;

protected:
    void forceUnlock();

protected:
    bool        _isLocked;
    int         _threadID;
    lock_type   _lock;
};


} // namespace cie::mp


#endif