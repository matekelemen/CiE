// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/concurrency/inc/ThreadPool.hpp"


namespace cie::mp {


ThreadPool::ThreadPool( Size size ) :
    _terminate( false )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( size == 0 )
        CIE_THROW( Exception, "Cannot create thread pool of size 0!" )

    Size maxNumberOfThreads = std::thread::hardware_concurrency();
    
    if ( maxNumberOfThreads == 0 )
        maxNumberOfThreads = 1;

    if ( maxNumberOfThreads < size )
        size = maxNumberOfThreads;

    // Initialize threads
    this->_threads.reserve( size );

    for ( Size i=0; i<size; ++i )
        this->_threads.push_back( std::thread(&ThreadPool::jobScheduler, this) );

    CIE_END_EXCEPTION_TRACING
}


ThreadPool::ThreadPool() :
    ThreadPool( std::thread::hardware_concurrency() )
{
}


ThreadPool::~ThreadPool()
{
    this->terminate();
}


void ThreadPool::queueJob( ThreadPool::job_type job )
{
    {
        std::unique_lock<ThreadPool::mutex_type> lock( this->_mutex );
        this->_jobs.push_back( job );
    }
    this->_condition.notify_one();
}


Size ThreadPool::size() const
{
    return this->_threads.size();
}


Size ThreadPool::numberOfJobs() const
{
    return this->_jobs.size();
}


void ThreadPool::terminate()
{
    {
        std::unique_lock<ThreadPool::mutex_type> lock( this->_mutex );
        this->_terminate = true;
        this->_condition.notify_all();
    }

    for ( auto& r_thread : this->_threads )
        r_thread.join();

    this->_threads.clear();
}


void ThreadPool::jobScheduler()
{
    while ( true )
    {
        ThreadPool::job_type job = nullptr;

        {
            std::unique_lock<ThreadPool::mutex_type> lock( this->_mutex );
            this->_condition.wait(
                lock,
                [this]{ return !this->_jobs.empty() || this->_terminate; }
            );

            if ( !this->_jobs.empty() )
            {
                job = this->_jobs.front();
                this->_jobs.pop_front();
            }
            else
                break;
        }

        if ( job != nullptr )
            job();
    }
}


} // namespace cie::mp