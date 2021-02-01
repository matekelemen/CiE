#ifndef CIE_UTILS_CONCURRENCY_THREAD_POOL_HPP
#define CIE_UTILS_CONCURRENCY_THREAD_POOL_HPP

// --- Internal Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <functional>


namespace cie::mp {


class ThreadPool
{
public:
    using thread_type      = std::thread;
    using job_type         = std::function<void()>;
    using thread_container = std::vector<thread_type>;
    using job_container    = std::deque<job_type>;

    using mutex_type       = std::mutex;

public:
    /// Create a thread pool with 'size' threads
    /**
     * @note 'size' must be positive and not greater than the maximum number
     * of threads on the system. The number of threads is capped at the supported
     * max.
     */
    ThreadPool( Size size );

    /// Create a thread pool with the maximum number of threads the system supports
    ThreadPool();

    ~ThreadPool();

    /// Queue a job for execution
    void queueJob( job_type job );

    /// Number of threads in the pool
    Size size() const;

    /// Number of jobs in the queue
    Size numberOfJobs() const;

    void terminate();

private:
    ThreadPool( const ThreadPool& r_rhs ) = delete;
    ThreadPool( ThreadPool&& r_rhs ) = delete;
    ThreadPool& operator=( const ThreadPool& r_rhs ) = delete;

    void jobScheduler();

private:
    bool                    _terminate;
    thread_container        _threads;
    job_container           _jobs;
    mutex_type              _mutex;
    std::condition_variable _condition;
};


} // namespace cie::mp


#endif