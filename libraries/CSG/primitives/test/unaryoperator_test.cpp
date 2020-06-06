// --- External Includes ---
#include "catch.hpp"

// --- Linalg Includes ---
#include <linalg/types.hpp>

// --- Internal Includes ---
#include "../inc/csgoperators.hpp"
#include "../inc/csgfactory.hpp"
#include "../inc/csgobject.hpp"

namespace cie {
namespace csg {


// Helper classes
const Size dimension = 2;

class CSGTestObject_default : public CSGObject<dimension,Bool,Double>
{
public:
    CSGTestObject_default() = default;

protected:
    Bool operator()(const typename CSGTestObject_default::point_type& point) const override
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
    Bool operator()(const typename CSGTestObject_nondefault::point_type& point) const override
    {
        return true;
    }
};


class TestUnaryOperator : public UnaryOperator<dimension,Bool,Double>
{
protected:
    Bool operator()( const typename TestUnaryOperator::point_type& point ) const override
    { return !this->_rhs->evaluate(point); }
};


TEST_CASE( "UnaryOperator", "[primitives]" )
{
    // Check sequential rhs emplacement
    TestUnaryOperator op;
    DoubleArray<dimension> doubleArray;

    CHECK_NOTHROW( op.emplaceRhs<CSGTestObject_default>() );
    CHECK( op.evaluate(doubleArray) == true );

    CHECK_NOTHROW( op.emplaceRhs<CSGTestObject_nondefault>( 0.0 ) );
    CHECK( op.evaluate(doubleArray) == false );

    CHECK_NOTHROW( op.emplaceRhs<CSGTestObject_nondefault>( 0.0, 0 ) );
    CHECK( op.evaluate(doubleArray) == false );

    // Check rvalue rhs binding
    CHECK_NOTHROW( op.bindRhs(CSGTestObject_default()) );
    CHECK( op.evaluate(doubleArray) == true );

    // Check pointer rhs binding
    CSGFactory<dimension> factory;
    CHECK_NOTHROW( op.bindRhsPtr<CSGTestObject_nondefault>( factory.make<CSGTestObject_nondefault>(0.0) ) );
    CHECK( op.evaluate(doubleArray) == false );
}


}
}