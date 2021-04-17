// --- Utility Includes ---
#include <cieutils/concurrency.hpp>
#include <cieutils/logging.hpp>
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <functional>
#include <iostream>


unsigned int crappyFibonacci( unsigned int index )
{
    if ( index < 3 )
        return 1;
    else
        return crappyFibonacci( index - 1 ) + crappyFibonacci( index - 2 );
}


namespace cie {


int main()
{
    utils::Logger log( OUTPUT_PATH / "thread_pool_benchmark.log", true );

    const unsigned int numberOfJobs = 1e6;
    const unsigned int fibonacciIndex = 18;

    log << "\e[38;2;0;255;0m";  // Green
    {
        auto localBlock = log.newBlock( "INFO" );
        localBlock << "Number of threads: " + std::to_string(mp::ThreadPool::maxNumberOfThreads());
        localBlock << "Number of jobs   : " + std::to_string(numberOfJobs);
        localBlock << "Job type         : recursive fibonacci";
        localBlock << "Job arguments    : " + std::to_string(fibonacciIndex);
    }
    log << "\e[0m";             // Reset color

    {
        auto localBlock = log.newBlock( "serial" );
        for ( unsigned int i=0; i<numberOfJobs; ++i )
            crappyFibonacci( fibonacciIndex );
    }

    {
        mp::ThreadPool pool;
        auto localBlock = log.newBlock( "ThreadPool - job queue" );

        for ( unsigned int i=0; i<numberOfJobs; ++i )
            pool.queueJob( std::bind(&crappyFibonacci, fibonacciIndex) );
        pool.terminate();
    }

    {
        auto localBlock = log.newBlock( "ThreadPool - parallel for" );

        mp::ParallelFor<>()(
            0, numberOfJobs, 1,
            [fibonacciIndex]( Size index ){ crappyFibonacci(fibonacciIndex); }
        );
    }

    #ifdef _OPENMP
    #pragma omp parallel
    {
        #pragma omp single
        {
            auto localBlock = log.newBlock( "OpenMP - tasks" );
            for ( unsigned int i=0; i<numberOfJobs; ++i )
            {
                #pragma omp task
                crappyFibonacci( fibonacciIndex );
            }
        }
    }

    {
        auto localBlock = log.newBlock( "OpenMP - parallel for" );
        #pragma omp parallel for
        for ( unsigned int i=0; i<numberOfJobs; ++i )
            crappyFibonacci( fibonacciIndex );
    }
    #endif

    return 0;
}


} // namespace cie




int main()
{
    cie::main();
    return 0;
}