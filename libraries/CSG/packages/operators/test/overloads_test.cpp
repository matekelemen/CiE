// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/operators/inc/overloads.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"


namespace cie::csg {


CIE_TEST_CASE( "overloads", "[operators]" )
{
    CIE_TEST_CASE_INIT( "overloads" )

    const Size Dimension = 2;
    using CoordinateType = Double;
    using ObjectPtr      = CSGObjectPtr<Dimension,Bool,CoordinateType>;
    using Primitive      = boolean::Cube<Dimension,CoordinateType>;
    using Point          = Primitive::point_type;

    Point base0 { 0.0, 0.0 };
    Point base1 { 0.5, 0.5 };
    CoordinateType length0 = 1.0;
    CoordinateType length1 = 1.0;

    ObjectPtr p_lhs( new Primitive(
        base0, length0
    ) );
    ObjectPtr p_rhs( new Primitive(
        base1, length1
    ) );

    ObjectPtr p_object;

    {
        CIE_TEST_CASE_INIT( "operator*" )

        Point truePoint { 0.75, 0.75 };
        Point falsePoint { 0.25, 0.25 };

        CIE_TEST_REQUIRE_NOTHROW( p_lhs * p_rhs );
        CIE_TEST_CHECK_NOTHROW( p_object = p_lhs * p_rhs );

        CIE_TEST_REQUIRE_NOTHROW( p_object->at(truePoint) );
        CIE_TEST_CHECK( p_object->at(truePoint) );
        CIE_TEST_CHECK( !p_object->at(falsePoint) );
    }

    {
        CIE_TEST_CASE_INIT( "operator+" )

        Point truePoint { 0.75, 0.75 };
        Point falsePoint { -0.25, -0.25 };

        CIE_TEST_REQUIRE_NOTHROW( p_lhs + p_rhs );
        CIE_TEST_CHECK_NOTHROW( p_object = p_lhs + p_rhs );

        CIE_TEST_REQUIRE_NOTHROW( p_object->at(truePoint) );
        CIE_TEST_CHECK( p_object->at(truePoint) );
        CIE_TEST_CHECK( !p_object->at(falsePoint) );
    }

    {
        CIE_TEST_CASE_INIT( "operator-" )

        Point truePoint { 0.25, 0.25 };
        Point falsePoint { 0.75, 0.75 };

        CIE_TEST_REQUIRE_NOTHROW( p_lhs - p_rhs );
        CIE_TEST_CHECK_NOTHROW( p_object = p_lhs - p_rhs );

        CIE_TEST_REQUIRE_NOTHROW( p_object->at(truePoint) );
        CIE_TEST_CHECK( p_object->at(truePoint) );
        CIE_TEST_CHECK( !p_object->at(falsePoint) );
    }
}


} // namespace cie::csg