// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/cell.hpp"


namespace cie::csg {


TEST_CASE( "boolean::CubeCell", "[trees]" )
{

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

    using TestCell = boolean::CubeCell<2,Double>;

    REQUIRE_NOTHROW( TestCell( DoubleArray<2>({1.0,1.0}), 2.0) );
    TestCell cell( DoubleArray<2>({2.0,2.0}), 2.0 );

    REQUIRE_NOTHROW( cell.evaluate( DoubleArray<2>({-1.0,0.0}) ) );
    CHECK( cell.evaluate( DoubleArray<2>({-1.0,0.0}) ) == false );
    CHECK( cell.evaluate( cell.base()) == true );

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




TEST_CASE( "boolean::BoxCell", "[trees]" )
{

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

    using TestCell = boolean::BoxCell<2,Double>;

    REQUIRE_NOTHROW( TestCell( DoubleArray<2>({1.0,1.0}), DoubleArray<2>({0.0,1.0}) ) );
    TestCell cell( DoubleArray<2>({0.0,1.0}), DoubleArray<2>({4.0,2.0}) );

    REQUIRE_NOTHROW( cell.evaluate( DoubleArray<2>({0.0,0.0}) ) );
    CHECK( cell.evaluate( DoubleArray<2>({0.0,0.0}) ) == false );
    CHECK( cell.evaluate( cell.base()) == true );

    #ifdef CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS
    REQUIRE_THROWS( cell.split( DoubleArray<2>({0.0,0.0}) ) );
    #endif
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