// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/concurrency/inc/ThreadPool.hpp"


namespace cie::mp {


ThreadPool::ThreadPool( Size size ) :
    _terminate( false )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( size == 0 )
        CIE_THROW( Exception, "Cannot create thread pool of size 0!" )

    Size maxNumberOfThreads = ThreadPool::maxNumberOfThreads();
    
    if ( maxNumberOfThreads == 0 )
        maxNumberOfThreads = 1;

    if ( maxNumberOfThreads < size )
        size = maxNumberOfThreads;

    _barrier = false;

    // Initialize threads
    this->_threads.reserve( size );

    for ( _numberOfActiveThreads=0; _numberOfActiveThreads<size; ++_numberOfActiveThreads )
        this->_threads.push_back( std::thread(&ThreadPool::jobScheduler, this) );

    CIE_END_EXCEPTION_TRACING
}


ThreadPool::ThreadPool() :
    ThreadPool( ThreadPool::maxNumberOfThreads() )
{
}


ThreadPool::~ThreadPool()
{
    this->terminate();
}


Size ThreadPool::maxNumberOfThreads()
{
    return std::thread::hardware_concurrency();
}


void ThreadPool::queueJob( ThreadPool::job_type job )
{
    {
        std::unique_lock<ThreadPool::mutex_type> lock( this->_mutex );
        this->_jobs.push_back( job );
    }
    this->_jobCondition.notify_one();
}


Size ThreadPool::size() const
{
    return this->_threads.size();
}


Size ThreadPool::numberOfJobs() const
{
    return this->_jobs.size();
}


void ThreadPool::barrier()
{
    CIE_BEGIN_EXCEPTION_TRACING

    std::unique_lock<ThreadPool::mutex_type> lock( this->_mutex );
    
    CIE_CHECK(
        !this->_barrier,
        "barrier was called on an ongoing barrier!"
    )

    this->_masterCondition.wait(
        lock,
        [this]{ return this->_numberOfActiveThreads == this->_threads.size(); }
    );

    this->_barrier = true;

    this->_jobCondition.notify_all();

    this->_masterCondition.wait(
        lock,
        [this]{ return (this->_terminate) || (this->_numberOfActiveThreads == 0); }
    );

    this->_barrier = false;
    this->_barrierCondition.notify_all();

    CIE_END_EXCEPTION_TRACING
}


void ThreadPool::terminate()
{
    {
        std::unique_lock<ThreadPool::mutex_type> lock( this->_mutex );
        this->_terminate = true;
        this->_jobCondition.notify_all();
        this->_barrierCondition.notify_all();
    }

    for ( auto& r_thread : this->_threads )
        r_thread.join();

    this->_threads.clear();
}


void ThreadPool::jobScheduler()
{
    auto id = this->threadID();

    while ( true )
    {
        ThreadPool::job_type job = nullptr;

        {
            std::unique_lock<ThreadPool::mutex_type> lock( this->_mutex );

            this->_jobCondition.wait(
                lock,
                [this]{ return !this->_jobs.empty() || this->_terminate || this->_barrier; }
            );

            // Got a job? -> strip it!
            if ( !this->_jobs.empty() )
            {
                job = this->_jobs.front();
                this->_jobs.pop_front();
            }
        }

        // Have a stripped job? -> execute it!
        if ( job )
            job();

        // Don't have a stripped job? -> terminate or set blocked for barrier
        else
        {
            std::unique_lock<ThreadPool::mutex_type> lock( this->_mutex );

            // Terminate thread
            if ( this->_terminate )
                break;

            // Enqueue at barrier
            else if ( this->_barrier )
            {
                if ( this->_numberOfActiveThreads )
                    --this->_numberOfActiveThreads;

                this->_masterCondition.notify_all();

                this->_barrierCondition.wait(
                    lock,
                    [this]{ return (!this->_barrier) || this->_terminate; }
                );

                ++this->_numberOfActiveThreads;
                this->_masterCondition.notify_all();
            }
            else
                CIE_THROW( Exception, "Thread has no job, no barrier, and is not terminated" )
        } // job queue empty
    }
}


Size ThreadPool::threadID() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it_threadEnd = this->_threads.end();
    auto id = std::this_thread::get_id();

    for ( auto it=this->_threads.begin(); it!=it_threadEnd; ++it )
        if ( it->get_id() == id )
            return std::distance( this->_threads.begin(), it );

    return std::distance( this->_threads.begin(), it_threadEnd );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::mp