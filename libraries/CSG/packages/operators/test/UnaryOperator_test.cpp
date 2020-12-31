// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/operators/inc/UnaryOperator.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"


namespace cie::csg {


class TestUnaryOperator : public UnaryOperator<2,Bool,Double>
{
public:
    template <class ...Args>
    TestUnaryOperator( Args&&... args ) :
        UnaryOperator<2,Bool,Double>( std::forward<Args>(args)... )
    {}

    Bool at( const TestUnaryOperator::point_type& r_point ) const override { return this->_p_rhs->at(r_point); }
};


CIE_TEST_CASE( "UnaryOperator", "[operators]" )
{
    CIE_TEST_CASE_INIT( "UnaryOperator" )

    const Size Dimension = 2;

    using CoordinateType = Double;
    using Operator       = TestUnaryOperator;
    using Primitive      = boolean::Cube<Dimension,CoordinateType>;
    using Point          = Primitive::point_type;

    Point base { 0.0, 0.0 };
    CoordinateType length = 1.0;

    Point truePoint { 0.5, 0.5 };
    Point falsePoint( {-0.5, -0.5} );

    CSGObjectPtr<Dimension,Bool,CoordinateType> p_primitive(
        new Primitive( base, length )
    );

    // Default constructor
    CIE_TEST_REQUIRE_NOTHROW( Operator() );
    Operator operator0;

    CIE_TEST_REQUIRE_NOTHROW( operator0.bindRhs(p_primitive) );

    CIE_TEST_CHECK_NOTHROW( operator0.at( truePoint ) );
    CIE_TEST_CHECK( operator0.at( truePoint ) );
    CIE_TEST_CHECK( !operator0.at( falsePoint ) );

    CIE_TEST_REQUIRE_NOTHROW( operator0.emplaceRhs<Primitive>( base, length ) );
    CIE_TEST_CHECK_NOTHROW( operator0.at( truePoint ) );
    CIE_TEST_CHECK( operator0.at( truePoint ) );
    CIE_TEST_CHECK( !operator0.at( falsePoint ) );

    // Constructor with operand
    CIE_TEST_REQUIRE_NOTHROW( Operator(p_primitive) );
    Operator operator1( p_primitive );

    CIE_TEST_CHECK_NOTHROW( operator1.at( truePoint ) );
    CIE_TEST_CHECK( operator1.at( truePoint ) );
    CIE_TEST_CHECK( !operator1.at( falsePoint ) );
}


} // namespace cie::csg