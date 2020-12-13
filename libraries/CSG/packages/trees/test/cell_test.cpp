// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/cell.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"
#include "cmake_variables.hpp"


namespace cie::csg {


CIE_TEST_CASE( "CubeCell", "[trees]" )
{
    CIE_TEST_CASE_INIT( "CubeCell" )
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

    using PrimitiveType = Cube<2,Double>;
    using TestCell      = CubeCell<PrimitiveType>;

    REQUIRE_NOTHROW( TestCell( DoubleArray<2>({1.0,1.0}), 2.0) );
    TestCell cell( DoubleArray<2>({2.0,2.0}), 2.0 );

    REQUIRE_NOTHROW( cell.split() );
    auto p_ConstructorArgumentContainer = cell.split();
    REQUIRE( p_ConstructorArgumentContainer->size() == 4 );

    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(0))[0] - 2.0 ) < 1e-16 ); // <-- base.x
    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(0))[1] - 2.0 ) < 1e-16 ); // <-- base.y
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(0)) - 1.0    ) < 1e-16 ); // <-- length

    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(1))[0] - 3.0 ) < 1e-16 ); // <-- base.x
    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(1))[1] - 2.0 ) < 1e-16 ); // <-- base.y
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(1)) - 1.0    ) < 1e-16 ); // <-- length

    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(2))[0] - 2.0 ) < 1e-16 ); // <-- base.x
    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(2))[1] - 3.0 ) < 1e-16 ); // <-- base.y
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(2)) - 1.0    ) < 1e-16 ); // <-- length

    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(3))[0] - 3.0 ) < 1e-16 ); // <-- base.x
    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(3))[1] - 3.0 ) < 1e-16 ); // <-- base.y
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(3)) - 1.0    ) < 1e-16 ); // <-- length

    for (const auto& constructorArguments : *p_ConstructorArgumentContainer)
        CHECK_NOTHROW( std::make_from_tuple<TestCell>(constructorArguments) );
}




CIE_TEST_CASE( "boolean::BoxCell", "[trees]" )
{
    CIE_TEST_CASE_INIT( "boolean::BoxCell" )
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
    using TestCell      = BoxCell<PrimitiveType>;

    REQUIRE_NOTHROW( TestCell( DoubleArray<2>({1.0,1.0}), DoubleArray<2>({0.0,1.0}) ) );
    TestCell cell( DoubleArray<2>({0.0,1.0}), DoubleArray<2>({4.0,2.0}) );

    REQUIRE_NOTHROW( cell.split(DoubleArray<2>({1.0,2.0})) );
    auto p_ConstructorArgumentContainer = cell.split( DoubleArray<2>({1.0,2.0}) );
    REQUIRE( p_ConstructorArgumentContainer->size() == 4 );

    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(0))[0] - 0.0) < 1e-16 ); // <-- base.x
    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(0))[1] - 1.0) < 1e-16 ); // <-- base.y
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(0))[0] - 1.0) < 1e-16 ); // <-- length.x
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(0))[1] - 1.0) < 1e-16 ); // <-- length.y

    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(1))[0] - 1.0) < 1e-16 ); // <-- base.x
    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(1))[1] - 1.0) < 1e-16 ); // <-- base.y
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(1))[0] - 3.0) < 1e-16 ); // <-- length.x
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(1))[1] - 1.0) < 1e-16 ); // <-- length.y

    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(2))[0] - 0.0) < 1e-16 ); // <-- base.x
    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(2))[1] - 2.0) < 1e-16 ); // <-- base.y
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(2))[0] - 1.0) < 1e-16 ); // <-- length.x
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(2))[1] - 1.0) < 1e-16 ); // <-- length.y

    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(3))[0] - 1.0) < 1e-16 ); // <-- base.x
    CHECK( std::abs( std::get<0>(p_ConstructorArgumentContainer->at(3))[1] - 2.0) < 1e-16 ); // <-- base.y
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(3))[0] - 3.0) < 1e-16 ); // <-- length.x
    CHECK( std::abs( std::get<1>(p_ConstructorArgumentContainer->at(3))[1] - 1.0) < 1e-16 ); // <-- length.y

    for (const auto& constructorArguments : *p_ConstructorArgumentContainer)
        CHECK_NOTHROW( std::make_from_tuple<TestCell>(constructorArguments) );
}


} // namespace cie::csg