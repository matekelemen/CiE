// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/testing/inc/ConstructorTracker.hpp"
#include "cieutils/packages/concurrency/inc/ThreadSafeMap.hpp"

// --- STL Includes ---
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <numeric>


namespace cie::mp {


struct TrackedInteger : public utils::ConstructorTracker<TrackedInteger>
{
    TrackedInteger() : _value(0) {}
    explicit TrackedInteger( int value ) : _value(value) {}
    const int get() const { return _value; }

private:
    int _value;
};


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
    CIE_TEST_CHECK( r_map.size() == maxCount );

    for ( const auto& r_pair : r_map )
        CIE_TEST_CHECK( r_pair.first == r_pair.second.get() );

    CIE_TEST_CHECK(
        std::accumulate( r_map.begin(),
                         r_map.end(),
                         Size(0),
                         [](Size lhs, const auto& r_pair){ return lhs + r_pair.first; } )
        ==
        maxCount * (maxCount-1) / 2
    );
}

} // namespace map


CIE_TEST_CASE( "ThreadSafeMap", "[concurrency]" )
{
    CIE_TEST_CASE_INIT( "ThreadSafeMap" )

    using KeyType         = int;
    using ValueType       = TrackedInteger;

    Size numberOfElements = 1e4;

    {
        CIE_TEST_CASE_INIT( "std::map base" )
        using MapBaseType   = std::map<KeyType,ValueType>;
        using MapType       = ThreadSafeMap<MapBaseType>;

        CIE_TEST_REQUIRE_NOTHROW( MapType() );
        MapType map;

        CIE_TEST_CHECK_NOTHROW( map.clear() );
        CIE_TEST_CHECK( map.empty() );

        map::fillTest( map, numberOfElements );
        map::checkContainer( map, numberOfElements );
    }

    {
        CIE_TEST_CASE_INIT( "std::unordered_map base" )
        using MapBaseType   = std::unordered_map<KeyType,ValueType>;
        using MapType       = ThreadSafeMap<MapBaseType>;

        CIE_TEST_REQUIRE_NOTHROW( MapType() );
        MapType map;

        CIE_TEST_CHECK_NOTHROW( map.clear() );
        CIE_TEST_CHECK( map.empty() );

        map::fillTest( map, numberOfElements );
        map::checkContainer( map, numberOfElements );
    }
}

} // namespace cie::mp
