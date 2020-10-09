// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "cieutils/packages/concurrency/inc/ThreadSafeMap.hpp"
#include "cieutils/packages/macros/inc/testing.hpp"

// --- STL Includes ---
#include <map>
#include <unordered_map>
#include <functional>


namespace cie::mp {


namespace map {


template <class MapType>
bool fillTest( MapType& r_map )
{
    CIE_TEST_CASE_INIT( "fill test" )

    int maxCount    = 1e2;

    #pragma omp parallel shared(r_map,maxCount)
    {
        for ( int i=0; i<maxCount; ++i )
        {
            r_map.try_emplace( i, i );
        }
    }

    return true;
}

} // namespace map


TEST_CASE( "ThreadSafeMap", "[concurrency]" )
{
    CIE_TEST_CASE_INIT( "ThreadSafeMap" )

    using KeyType       = int;
    using ValueType     = double;

    {
        CIE_TEST_CASE_INIT( "std::map base" )
        using MapBaseType   = std::map<KeyType,ValueType>;
        using MapType       = ThreadSafeMap<MapBaseType>;

        REQUIRE_NOTHROW( MapType() );
        MapType map;

        CHECK_NOTHROW( map.clear() );
        CHECK( map.empty() );

        map::fillTest( map );
    }
}

} // namespace cie::mp
