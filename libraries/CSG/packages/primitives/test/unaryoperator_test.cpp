// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Linalg Includes ---
#include "linalg/packages/types/inc/arraytypes.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgoperators.hpp"
#include "CSG/packages/primitives/inc/csgfactory.hpp"
#include "CSG/packages/primitives/inc/csgobject.hpp"

namespace cie::csg {


// Helper classes
const Size dimension = 2;

class CSGTestObject_default : public CSGObject<dimension,Bool,Double>
{
public:
    CSGTestObject_default() = default;

protected:
    Bool at(const typename CSGTestObject_default::point_type& point) const override
    {
        return false;
    }
};


class CSGTestObject_nondefault : public CSGObject<dimension,Bool,Double>
{
public:
    CSGTestObject_nondefault() = delete;
    CSGTestObject_nondefault( Double arg0 ) {}
    CSGTestObject_nondefault( Double arg0, Size arg1 ) {}

protected:
    Bool at(const typename CSGTestObject_nondefault::point_type& point) const override
    {
        return true;
    }
};


class TestUnaryOperator : public UnaryOperator<dimension,Bool,Double>
{
protected:
    Bool at( const typename TestUnaryOperator::point_type& point ) const override
    { return !this->_rhs->evaluate(point); }
};


CIE_TEST_CASE( "UnaryOperator", "[primitives]" )
{
    CIE_TEST_CASE_INIT( "UnaryOperator" )

    // Check sequential rhs emplacement
    TestUnaryOperator op;
    DoubleArray<dimension> doubleArray;

    CIE_TEST_CHECK_NOTHROW( op.emplaceRhs<CSGTestObject_default>() );
    CIE_TEST_CHECK( op.evaluate(doubleArray) == true );

    CIE_TEST_CHECK_NOTHROW( op.emplaceRhs<CSGTestObject_nondefault>( 0.0 ) );
    CIE_TEST_CHECK( op.evaluate(doubleArray) == false );

    CIE_TEST_CHECK_NOTHROW( op.emplaceRhs<CSGTestObject_nondefault>( 0.0, 0 ) );
    CIE_TEST_CHECK( op.evaluate(doubleArray) == false );

    // Check rvalue rhs binding
    CIE_TEST_CHECK_NOTHROW( op.bindRhs(CSGTestObject_default()) );
    CIE_TEST_CHECK( op.evaluate(doubleArray) == true );

    // Check pointer rhs binding
    CSGFactory<dimension> factory;
    CIE_TEST_CHECK_NOTHROW( op.bindRhsPtr<CSGTestObject_nondefault>( factory.make<CSGTestObject_nondefault>(0.0) ) );
    CIE_TEST_CHECK( op.evaluate(doubleArray) == false );
}


} // namespace cie::csg