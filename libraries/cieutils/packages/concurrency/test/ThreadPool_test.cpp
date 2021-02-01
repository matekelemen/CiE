// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "cieutils/packages/concurrency/inc/ThreadPool.hpp"

// --- STL Includes ---
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>


namespace cie::mp {


CIE_TEST_CASE( "ThreadPool", "[concurrency]" )
{
    CIE_TEST_CASE_INIT( "ThreadPool" )

    ThreadPool pool;
    std::vector<std::thread::id> threadIDs( 2 * pool.size() );

    for ( Size index=0; index<threadIDs.size(); ++index )
    {
        auto job = [index, &threadIDs]() -> void
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            threadIDs[index] = std::this_thread::get_id();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        };

        pool.queueJob( job );
    }
    pool.terminate();
    
    for ( auto threadID : threadIDs )
        CIE_TEST_CHECK( std::count( threadIDs.begin(), threadIDs.end(), threadID ) == 2 );
}


} // namespace cie::mp