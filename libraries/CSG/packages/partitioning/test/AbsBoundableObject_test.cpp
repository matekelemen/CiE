// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/partitioning/inc/AbsBoundableObject.hpp"


namespace cie::csg {


class TestBoundableObject : public AbsBoundableObject<1,Double>
{
public:
    TestBoundableObject() :
        AbsBoundableObject<1,Double>(),
        _counter( 0 )
    { this->computeBoundingBox(); }

    const Size counter() const 
    { return this->_counter; }

    void setBoundingBoxShouldRecompute()
    { this->boundingBoxShouldRecompute(); }

private:
    void computeBoundingBox_impl( typename TestBoundableObject::bounding_box& r_box ) override
    {
        this->_counter++;
        r_box.base() = { Double(this->_counter) };
        r_box.lengths() = { Double(this->_counter) };
    }

private:
    Size _counter;
};


template <class T>
concept NotBoxBoundable
= !concepts::BoxBoundable<T>;


template <concepts::BoxBoundable T>
bool isBoxBoundable( const T& r_object )
{ return true; }


template <NotBoxBoundable T>
bool isBoxBoundable( const T& r_object )
{ return false; }




CIE_TEST_CASE( "AbsBoundableObject", "[partitioning]" )
{
    CIE_CIE_TEST_CASE_INIT( "AbsBoundableObject" )


    REQUIRE_NOTHROW( TestBoundableObject() );
    TestBoundableObject test;
    const TestBoundableObject& r_test = test;

    CHECK( isBoxBoundable(test) );
    CHECK( !isBoxBoundable(0) );

    CHECK( test.counter() == 1 );

    REQUIRE_NOTHROW( test.boundingBox() );
    CHECK_NOTHROW( r_test.boundingBox() );

    REQUIRE( test.boundingBox().base().size() > 0 );
    CHECK( test.boundingBox().base().size() == 1 );
    CHECK( test.boundingBox().base()[0] == Approx(1.0) );

    REQUIRE( test.boundingBox().lengths().size() > 0 );
    CHECK( test.boundingBox().lengths().size() == 1 );
    CHECK( test.boundingBox().lengths()[0] == Approx(1.0) );

    CHECK_NOTHROW( test.setBoundingBoxShouldRecompute() );
    CHECK( test.counter() == 1 );

    REQUIRE_NOTHROW( test.boundingBox() );
    CHECK_NOTHROW( r_test.boundingBox() );

    CHECK( test.counter() == 2 );

    REQUIRE( test.boundingBox().base().size() > 0 );
    CHECK( test.boundingBox().base().size() == 1 );
    CHECK( test.boundingBox().base()[0] == Approx(2.0) );

    REQUIRE( test.boundingBox().lengths().size() > 0 );
    CHECK( test.boundingBox().lengths().size() == 1 );
    CHECK( test.boundingBox().lengths()[0] == Approx(2.0) );

    CHECK_NOTHROW( test.setBoundingBoxShouldRecompute() );
    CHECK_THROWS( r_test.boundingBox() );
    CHECK( test.counter() == 2 );
}


} // namespace cie::csg