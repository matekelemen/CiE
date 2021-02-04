// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/concurrency/inc/ParallelFor.hpp"


namespace cie::mp {


CIE_TEST_CASE( "ParallelFor", "[concurrency]" )
{
    CIE_TEST_CASE_INIT( "ParallelFor" )

    int firstPrivateVar = 100;
    Size indexMin       = 5;
    Size indexMax       = 105;
    Size stepSize       = 2;

    ParallelFor<>::firstPrivate( firstPrivateVar )(
        indexMin,
        indexMax,
        stepSize,
        []( Size index, int& privateVar )
        {
            std::cout << index << std::endl;
        }
    );
}


} // namespace cie::mp