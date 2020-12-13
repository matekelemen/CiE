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
    CIE_TEST_CASE_INIT( "AbsBoundableObject" )


    CIE_TEST_REQUIRE_NOTHROW( TestBoundableObject() );
    TestBoundableObject test;
    const TestBoundableObject& r_test = test;

    CIE_TEST_CHECK( isBoxBoundable(test) );
    CIE_TEST_CHECK( !isBoxBoundable(0) );

    CIE_TEST_CHECK( test.counter() == 1 );

    CIE_TEST_REQUIRE_NOTHROW( test.boundingBox() );
    CIE_TEST_CHECK_NOTHROW( r_test.boundingBox() );

    CIE_TEST_REQUIRE( test.boundingBox().base().size() > 0 );
    CIE_TEST_CHECK( test.boundingBox().base().size() == 1 );
    CIE_TEST_CHECK( test.boundingBox().base()[0] == Approx(1.0) );

    CIE_TEST_REQUIRE( test.boundingBox().lengths().size() > 0 );
    CIE_TEST_CHECK( test.boundingBox().lengths().size() == 1 );
    CIE_TEST_CHECK( test.boundingBox().lengths()[0] == Approx(1.0) );

    CIE_TEST_CHECK_NOTHROW( test.setBoundingBoxShouldRecompute() );
    CIE_TEST_CHECK( test.counter() == 1 );

    CIE_TEST_REQUIRE_NOTHROW( test.boundingBox() );
    CIE_TEST_CHECK_NOTHROW( r_test.boundingBox() );

    CIE_TEST_CHECK( test.counter() == 2 );

    CIE_TEST_REQUIRE( test.boundingBox().base().size() > 0 );
    CIE_TEST_CHECK( test.boundingBox().base().size() == 1 );
    CIE_TEST_CHECK( test.boundingBox().base()[0] == Approx(2.0) );

    CIE_TEST_REQUIRE( test.boundingBox().lengths().size() > 0 );
    CIE_TEST_CHECK( test.boundingBox().lengths().size() == 1 );
    CIE_TEST_CHECK( test.boundingBox().lengths()[0] == Approx(2.0) );

    CIE_TEST_CHECK_NOTHROW( test.setBoundingBoxShouldRecompute() );
    CIE_TEST_CHECK_THROWS( r_test.boundingBox() );
    CIE_TEST_CHECK( test.counter() == 2 );
}


} // namespace cie::csg