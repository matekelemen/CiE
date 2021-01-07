// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/Cell.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"
#include "cmake_variables.hpp"


namespace cie::csg {


CIE_TEST_CASE( "Cell", "[trees]" )
{
    CIE_TEST_CASE_INIT( "Cell" )
    /**
     *    y
     *    ^
     *    |
     * 
     *    4 -----------
     *      |    |    |
     *      |    |    |
     *    3 |----|----|
     *      |    |    |
     *      |    |    |
     *    2 -----------
     *      2    3    4  ->x
    */

    {
        CIE_TEST_CASE_INIT( "box Cell" )
        using PrimitiveType = Cube<2,Double>;
        using TestCell      = Cell<PrimitiveType>;

        CIE_TEST_REQUIRE_NOTHROW( TestCell( TestCell::point_type({1.0,1.0}), 2.0) );
        TestCell cell( TestCell::point_type({2.0,2.0}), 2.0 );

        CIE_TEST_REQUIRE_NOTHROW( cell.split() );
        auto p_ConstructorArgumentContainer = cell.split();
        CIE_TEST_REQUIRE( p_ConstructorArgumentContainer->size() == 4 );

        CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(0))[0] - 2.0 ) < 1e-16 ); // <-- base.x
        CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(0))[1] - 2.0 ) < 1e-16 ); // <-- base.y
        CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(0)) - 1.0    ) < 1e-16 ); // <-- length

        CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(1))[0] - 3.0 ) < 1e-16 ); // <-- base.x
        CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(1))[1] - 2.0 ) < 1e-16 ); // <-- base.y
        CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(1)) - 1.0    ) < 1e-16 ); // <-- length

        CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(2))[0] - 2.0 ) < 1e-16 ); // <-- base.x
        CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(2))[1] - 3.0 ) < 1e-16 ); // <-- base.y
        CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(2)) - 1.0    ) < 1e-16 ); // <-- length

        CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(3))[0] - 3.0 ) < 1e-16 ); // <-- base.x
        CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(3))[1] - 3.0 ) < 1e-16 ); // <-- base.y
        CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(3)) - 1.0    ) < 1e-16 ); // <-- length

        for (const auto& constructorArguments : *p_ConstructorArgumentContainer)
            CIE_TEST_CHECK_NOTHROW( std::make_from_tuple<TestCell>(constructorArguments) );
    }
}




CIE_TEST_CASE( "boolean::Cell", "[trees]" )
{
    CIE_TEST_CASE_INIT( "boolean::Cell" )
    /**
     *    y
     *    ^
     *    |
     * 
     *    3 ---------------------
     *      |    |              |
     *      |    |              |
     *    2 |----|--------------|
     *      |    |              |
     *      |    |              |
     *    1 ---------------------
     *      0    1    2    3    4  ->x
    */

    using PrimitiveType = Box<2,Double>;
    using TestCell      = Cell<PrimitiveType>;

    CIE_TEST_REQUIRE_NOTHROW( TestCell( TestCell::point_type({1.0,1.0}), TestCell::point_type({0.0,1.0}) ) );
    TestCell cell( TestCell::point_type({0.0,1.0}), TestCell::point_type({4.0,2.0}) );

    CIE_TEST_REQUIRE_NOTHROW( cell.split(TestCell::point_type({1.0,2.0})) );
    auto p_ConstructorArgumentContainer = cell.split( TestCell::point_type({1.0,2.0}) );
    CIE_TEST_REQUIRE( p_ConstructorArgumentContainer->size() == 4 );

    CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(0))[0] - 0.0) < 1e-16 ); // <-- base.x
    CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(0))[1] - 1.0) < 1e-16 ); // <-- base.y
    CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(0))[0] - 1.0) < 1e-16 ); // <-- length.x
    CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(0))[1] - 1.0) < 1e-16 ); // <-- length.y

    CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(1))[0] - 1.0) < 1e-16 ); // <-- base.x
    CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(1))[1] - 1.0) < 1e-16 ); // <-- base.y
    CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(1))[0] - 3.0) < 1e-16 ); // <-- length.x
    CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(1))[1] - 1.0) < 1e-16 ); // <-- length.y

    CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(2))[0] - 0.0) < 1e-16 ); // <-- base.x
    CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(2))[1] - 2.0) < 1e-16 ); // <-- base.y
    CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(2))[0] - 1.0) < 1e-16 ); // <-- length.x
    CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(2))[1] - 1.0) < 1e-16 ); // <-- length.y

    CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(3))[0] - 1.0) < 1e-16 ); // <-- base.x
    CIE_TEST_CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(3))[1] - 2.0) < 1e-16 ); // <-- base.y
    CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(3))[0] - 3.0) < 1e-16 ); // <-- length.x
    CIE_TEST_CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(3))[1] - 1.0) < 1e-16 ); // <-- length.y

    for (const auto& constructorArguments : *p_ConstructorArgumentContainer)
        CIE_TEST_CHECK_NOTHROW( std::make_from_tuple<TestCell>(constructorArguments) );
}


} // namespace cie::csg