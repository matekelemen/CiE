// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/concurrency/inc/ParallelFor.hpp"

// --- STL Includes ---
#include <vector>
#include <string>


namespace cie::mp {


CIE_TEST_CASE( "ParallelFor", "[concurrency]" )
{
    CIE_TEST_CASE_INIT( "ParallelFor" )

    int firstPrivateVar = 100;
    Size indexMin       = 5;
    Size indexMax       = 105;
    Size stepSize       = 2;

    {
        CIE_TEST_CASE_INIT( "index loop" )

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

    {
        CIE_TEST_CASE_INIT( "object loop" )

        std::vector<std::string> objects;
        objects.reserve( indexMax - indexMin );

        for ( Size i=0; i<indexMax-indexMin; ++i )
            objects.push_back( std::to_string(i) );

        std::string suffix = "_";

        ParallelFor<>::firstPrivate( suffix )(
            objects,
            []( std::string& r_item, std::string& r_suffix ) -> void
            {
                r_item += r_suffix;
            }
        );

        for ( Size i=0; i<objects.size(); ++i )
            CIE_TEST_CHECK(
                objects[i]
                ==
                std::to_string(i) + "_"
            );
    }
}


} // namespace cie::mp