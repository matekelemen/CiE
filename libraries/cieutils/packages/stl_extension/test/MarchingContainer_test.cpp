// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/stl_extension/inc/MarchingContainer.hpp"


namespace cie::utils {


CIE_TEST_CASE( "MarchingContainer", "[stl_extension]" )
{
    CIE_CIE_TEST_CASE_INIT( "MarchingContainer" )

    using ValueType     = int;
    using ContainerType = MarchingContainer<ValueType>;

    for ( Size capacity=3; capacity<7; ++capacity )
    {
        CIE_CIE_TEST_CASE_INIT( "capacity " + std::to_string(capacity) )

        {
            REQUIRE_NOTHROW( ContainerType(capacity) );
            ContainerType container( capacity );

            ValueType counter = 0;
            CHECK_NOTHROW( container.push_back( ++counter ) );
            CHECK_NOTHROW( container.emplace_back( ++counter ) );
            
            CHECK_NOTHROW( container.back() );
            CHECK( container.back() == counter );

            CHECK_NOTHROW( container.back() = ++counter );
            CHECK( container.back() == counter );
            auto tmp = container.back();

            CHECK_NOTHROW( container[0] );
            CHECK( container[0] == 1 );

            CHECK_NOTHROW( container[0] = ++counter );
            CHECK( container[0] == counter );

            CHECK_NOTHROW( container[1] );
            CHECK( container[1] == tmp );

            CHECK_NOTHROW( container[1] = ++counter );
            CHECK( container[1] == counter );

            CHECK_NOTHROW( container.size() );
            CHECK( container.size() == 2 );

            CHECK_NOTHROW( container[ container.size() - 1 ] );
            CHECK( container[ container.size()-1 ] == container.back() );

            #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
            CHECK_THROWS( container[container.size()] );
            #endif

            for ( Size i=0; i<3*capacity; ++i )
                CHECK_NOTHROW( container.push_back( ValueType(++counter) ) );

            #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
            CHECK_THROWS( container[ container.size() - capacity - 1 ] );
            #endif

            for ( Size i=0; i<capacity; ++i )
            {
                Size index = container.size() - capacity + i;
                REQUIRE_NOTHROW( container[index] );
                CHECK( container[index] == ValueType(counter-capacity+i+1) );
            }

            CHECK( container.back() == counter );

            #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
            CHECK_THROWS( container[container.size()] );
            #endif
        }

        {
            ValueType initializer = 12;
            ContainerType container( capacity, initializer );

            CHECK( container.size() == capacity );
            
            for ( Size i=0; i<capacity; ++i )
            {
                CHECK_NOTHROW( container[i] );
                CHECK( container[i] == initializer );
            }

            for ( Size i=capacity; i<4*capacity+1; ++i )
                CHECK_NOTHROW( container.emplace_back( ValueType(i) ) );

            for ( Size i=container.size()-capacity; i<container.size(); ++i )
            {
                CHECK_NOTHROW( container[i] );
                CHECK( container[i] == ValueType(i) );
            }
        }
    }
}


} // namespace cie::utils