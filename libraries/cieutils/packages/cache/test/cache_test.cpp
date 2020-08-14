// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/cache.hpp"

// --- STL Includes ---
#include <vector>
#include <array>


namespace cie::utils
{


TEST_CASE( "Cache", "[cache]" )
{
    typedef std::array<int,3>                       InputContainer;
    typedef std::vector<std::pair<int,int>>         OutputContainer;
    typedef Cache<InputContainer,OutputContainer>   TestCache;

    std::vector<InputContainer> inputs =
    {
        InputContainer( {1, 2, 3} ),
        InputContainer( {-1, -2, -3} ),
        InputContainer( {1, 4, 9} )
    };

    std::vector<OutputContainer> outputs;
    for (auto& input : inputs)
    {
        outputs.push_back( OutputContainer() );
        auto& output = outputs.back();
        for (auto& value : input)
            output.emplace_back( 2*value, value*value );
    }

    REQUIRE_NOTHROW( TestCache() );
    TestCache cache;

    CHECK_THROWS( cache[0] );

    REQUIRE( inputs.size() == outputs.size() );
    for (Size i=0; i<inputs.size(); ++i)
        CHECK_NOTHROW( cache.insert(inputs[i],outputs[i]) );

    for (Size i=0; i<inputs.size(); ++i)
    {
        const OutputContainer& output = cache[inputs[i]];
        REQUIRE( output.size() == outputs[i].size() );
        for (Size j=0; j<output.size(); ++j)
        {
            CHECK( output[j].first == outputs[i][j].first );
            CHECK( output[j].second == outputs[i][j].second );
        }
    }
}


}