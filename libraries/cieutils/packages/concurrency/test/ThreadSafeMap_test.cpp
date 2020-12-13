// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/concurrency/inc/ThreadSafeMap.hpp"

// --- STL Includes ---
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <numeric>


namespace cie::mp {


namespace map {

template <class MapType>
void fillTest( MapType& r_map, Size maxCount )
{
    CIE_TEST_CASE_INIT( "fill test" )

    #pragma omp parallel shared(r_map,maxCount)
    {
        for ( int i=0; i<int(maxCount); ++i )
        {
            r_map.try_emplace( i, i );
        }
    }
}

template <class MapType>
void checkContainer( MapType& r_map, Size maxCount )
{
    CHECK( r_map.size() == maxCount );

    for ( const auto& r_pair : r_map )
        CHECK( r_pair.first == r_pair.second );

    CHECK(
        std::accumulate( r_map.begin(),
                         r_map.end(),
                         Size(0),
                         [](Size lhs, const auto& r_pair){ return lhs + r_pair.first; } )
        ==
        maxCount * (maxCount-1) / 2
    );
}

} // namespace map


TEST_CASE( "ThreadSafeMap", "[concurrency]" )
{
    CIE_TEST_CASE_INIT( "ThreadSafeMap" )

    using KeyType         = int;
    using ValueType       = double;

    Size numberOfElements = 1e4;

    {
        CIE_TEST_CASE_INIT( "std::map base" )
        using MapBaseType   = std::map<KeyType,ValueType>;
        using MapType       = ThreadSafeMap<MapBaseType>;

        REQUIRE_NOTHROW( MapType() );
        MapType map;

        CHECK_NOTHROW( map.clear() );
        CHECK( map.empty() );

        map::fillTest( map, numberOfElements );
        map::checkContainer( map, numberOfElements );
    }

    {
        CIE_TEST_CASE_INIT( "std::unordered_map base" )
        using MapBaseType   = std::unordered_map<KeyType,ValueType>;
        using MapType       = ThreadSafeMap<MapBaseType>;

        REQUIRE_NOTHROW( MapType() );
        MapType map;

        CHECK_NOTHROW( map.clear() );
        CHECK( map.empty() );

        map::fillTest( map, numberOfElements );
        map::checkContainer( map, numberOfElements );
    }
}

} // namespace cie::mp
