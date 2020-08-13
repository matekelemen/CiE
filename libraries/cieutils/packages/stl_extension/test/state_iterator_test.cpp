// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/state_iterator.hpp"

// --- STL Includes ---
#include <vector>
#include <array>


namespace cie::utils {


TEST_CASE( "StateIncrementer", "[stl_extension]" )
{
    typedef StateIncrementer< std::vector<std::vector<int>> >   Incrementer;
} // TEST_CASE( "StateIncrementer")


} // namespace cie::utils