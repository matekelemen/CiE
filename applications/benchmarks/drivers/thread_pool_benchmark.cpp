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
        mp::ThreadPool pool;
        localBlock << "Number of threads: " + std::to_string(pool.size());
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
        auto localBlock = log.newBlock( "ThreadPool" );

        mp::ThreadPool pool;
        for ( unsigned int i=0; i<numberOfJobs; ++i )
            pool.queueJob( std::bind(&crappyFibonacci, fibonacciIndex) );
        pool.terminate();
    }
}


} // namespace cie




int main()
{
    cie::main();
    return 0;
}