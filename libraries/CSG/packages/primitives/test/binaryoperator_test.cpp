// --- External Includes ---
#include "catch.hpp"

// --- Linalg Includes ---
#include "linalg/packages/types/inc/arraytypes.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgoperators.hpp"
#include "CSG/packages/primitives/inc/csgobject.hpp"

namespace cie::csg {


class CSGTestObject_true : public CSGObject<2,Bool,Double>
{
protected:
    Bool at(const typename CSGTestObject_true::point_type& point) const override
    {
        return true;
    }
};


class CSGTestObject_false : public CSGObject<2,Bool,Double>
{
protected:
    Bool at(const typename CSGTestObject_false::point_type& point) const override
    {
        return false;
    }
};


class TestBinaryOperator : public BinaryOperator<2,Bool,Double>
{
protected:
    Bool at( const typename TestBinaryOperator::point_type& point ) const override
    { return this->_lhs->evaluate(point) ^ this->_rhs->evaluate(point); }
};


TEST_CASE( "BinaryOperator", "[primitives]" )
{
    REQUIRE_NOTHROW( TestBinaryOperator() );
    TestBinaryOperator op;
    DoubleArray<2> doubleArray;

    CHECK_NOTHROW(op.emplaceLhs<CSGTestObject_true>());
    CHECK_NOTHROW(op.emplaceRhs<CSGTestObject_false>());
    CHECK( op.evaluate(doubleArray) == true );

    CHECK_NOTHROW(op.emplaceRhs<CSGTestObject_true>());
    CHECK( op.evaluate(doubleArray) == false );

    CHECK_NOTHROW(op.emplaceLhs<CSGTestObject_false>());
    CHECK( op.evaluate(doubleArray) == true );

    CHECK_NOTHROW(op.emplaceRhs<CSGTestObject_false>());
    CHECK( op.evaluate(doubleArray) == false );
}


} // namespace cie::csg