// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/operators/inc/BinaryOperator.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"


namespace cie::csg {


class TestBinaryOperator : public BinaryOperator<2,Bool,Double>
{
public:
    template <class ...Args>
    TestBinaryOperator( Args&&... args ) :
        BinaryOperator<2,Bool,Double>( std::forward<Args>(args)... )
    {}

    Bool at( const TestBinaryOperator::point_type& r_point ) const override
        { return this->_p_lhs->at(r_point) && !this->_p_rhs->at(r_point); }
};


CIE_TEST_CASE( "BinaryOperator", "[operators]" )
{
    CIE_TEST_CASE_INIT( "BinaryOperator" )

    const Size Dimension = 2;

    using CoordinateType = Double;
    using Operator       = TestBinaryOperator;
    using Primitive      = boolean::Cube<Dimension,CoordinateType>;
    using Point          = Primitive::point_type;

    Point base0 { 0.0, 0.0 };
    Point base1 { 0.25, 0.25 };
    CoordinateType length0 = 1.0;
    CoordinateType length1 = 0.75;

    Point truePoint( {0.1, 0.1} );
    Point falsePoint { 0.5, 0.5 };

    CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs(
        new Primitive( base0, length0 )
    );
    CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs(
        new Primitive( base1, length1 )
    );

    // Default constructor
    CIE_TEST_REQUIRE_NOTHROW( Operator() );
    Operator operator0;

    CIE_TEST_REQUIRE_NOTHROW( operator0.bindLhs(p_lhs) );
    CIE_TEST_REQUIRE_NOTHROW( operator0.bindRhs(p_rhs) );

    CIE_TEST_CHECK_NOTHROW( operator0.at( truePoint ) );
    CIE_TEST_CHECK( operator0.at( truePoint ) );
    CIE_TEST_CHECK( !operator0.at( falsePoint ) );

    CIE_TEST_REQUIRE_NOTHROW( operator0.emplaceLhs<Primitive>( base0, length0 ) );
    CIE_TEST_REQUIRE_NOTHROW( operator0.emplaceRhs<Primitive>( base1, length1 ) );
    CIE_TEST_CHECK_NOTHROW( operator0.at( truePoint ) );
    CIE_TEST_CHECK( operator0.at( truePoint ) );
    CIE_TEST_CHECK( !operator0.at( falsePoint ) );

    // Constructor with operand
    CIE_TEST_REQUIRE_NOTHROW( Operator(p_lhs,p_rhs) );
    Operator operator1( p_lhs, p_rhs );

    CIE_TEST_CHECK_NOTHROW( operator1.at( truePoint ) );
    CIE_TEST_CHECK( operator1.at( truePoint ) );
    CIE_TEST_CHECK( !operator1.at( falsePoint ) );
}


} // namespace cie::csg