#ifndef CIE_UTILS_CONCURRENCY_ABS_MUTEX_HPP
#define CIE_UTILS_CONCURRENCY_ABS_MUTEX_HPP


namespace cie::mp {


/**
 * Interface for mutex implementations
 */
class AbsMutex
{
public:
    AbsMutex();
    AbsMutex( const AbsMutex& r_rhs ) = delete;
    AbsMutex& operator=( const AbsMutex& r_rhs ) = delete;

    void lock();
    void unlock();

    bool isLocked() const;

protected:
    virtual void lock_impl() = 0;
    virtual void unlock_impl() = 0;

private:
    bool _isLocked;
};


} // namespace cie::mp


#endif