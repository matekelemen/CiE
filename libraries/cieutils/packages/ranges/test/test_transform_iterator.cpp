// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/ranges/inc/transform_iterator.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>


namespace cie::utils {


// Helper class for testing
template <class IteratorType>
class TransformIteratorHelper :
    public TransformIterator<   uint8_t,
                                char,
                                IteratorType,
                                TransformIteratorHelper<IteratorType> >
{
public:
    TransformIteratorHelper( IteratorType base ) : 
        TransformIteratorHelper<IteratorType>::transform_iterator_type(base), 
        _value(*base) {}
protected:
    virtual typename TransformIteratorHelper<IteratorType>::reference convert() override { _value = *this->_base; return _value; }
    virtual typename TransformIteratorHelper<IteratorType>::self_type* thisPtr() override {return this;}
    char _value;
};



CIE_TEST_CASE( "TransformIterator", "[ranges]" )
{
    std::vector<int> base = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    typedef TransformIteratorHelper<decltype(base)::iterator> TrIterator;

    // Test constructor
    CIE_TEST_REQUIRE_NOTHROW( std::make_shared<TrIterator>(base.begin()) );
    TrIterator begin(base.begin());

    // Test dereferencing
    CIE_TEST_REQUIRE_NOTHROW( *begin );
    CIE_TEST_CHECK( *begin == char(base[0]) );

    // Test pre increment
    CIE_TEST_REQUIRE_NOTHROW( ++begin );
    CIE_TEST_CHECK( *begin == char(base[1]) );

    // Test pre decrement
    CIE_TEST_REQUIRE_NOTHROW( --begin );
    CIE_TEST_CHECK( *begin == char(base[0]) );

    // Test repeated pre increments/decremenets
    CIE_TEST_CHECK_NOTHROW( ++begin );
    CIE_TEST_CHECK_NOTHROW( ++begin );
    CIE_TEST_CHECK( *begin == char(base[2]) );

    CIE_TEST_CHECK_NOTHROW( --begin );
    CIE_TEST_CHECK_NOTHROW( --begin );
    CIE_TEST_CHECK( *begin == char(base[0]) );

    CIE_TEST_CHECK_NOTHROW( ++begin );
    CIE_TEST_CHECK_NOTHROW( --begin );
    CIE_TEST_CHECK( *begin == char(base[0]) );

    // Test post increment
    CIE_TEST_REQUIRE_NOTHROW( begin++ );
    CIE_TEST_CHECK( *begin == char(base[1]) );

    // Test post decrement
    CIE_TEST_REQUIRE_NOTHROW( begin-- );
    CIE_TEST_CHECK( *begin == char(base[0]) );

    // Test repeated post increments/decremenets
    CIE_TEST_CHECK_NOTHROW( begin++ );
    CIE_TEST_CHECK_NOTHROW( begin++ );
    CIE_TEST_CHECK( *begin == char(base[2]) );

    CIE_TEST_CHECK_NOTHROW( begin-- );
    CIE_TEST_CHECK_NOTHROW( begin-- );
    CIE_TEST_CHECK( *begin == char(base[0]) );

    CIE_TEST_CHECK_NOTHROW( begin++ );
    CIE_TEST_CHECK_NOTHROW( begin-- );
    CIE_TEST_CHECK( *begin == char(base[0]) );

    // Check member operator overloads
    CIE_TEST_REQUIRE_NOTHROW( begin += 5 );
    CIE_TEST_CHECK( *begin == char(base[5]) );
    CIE_TEST_REQUIRE_NOTHROW( begin -= 2 );
    CIE_TEST_CHECK( *begin == char(base[3]) );

    // Check operator overloads
    CIE_TEST_REQUIRE_NOTHROW( begin + 5 );
    CIE_TEST_REQUIRE_NOTHROW( *(begin + 5) );
    CIE_TEST_CHECK( *(begin + 5) == char(base[8]) );

    CIE_TEST_REQUIRE_NOTHROW( begin - 2 );
    CIE_TEST_REQUIRE_NOTHROW( *(begin - 2) );
    CIE_TEST_CHECK( *(begin - 2) == char(base[1]) );

    // Check boolean operators (non-exhaustive)
    CIE_TEST_REQUIRE_NOTHROW( begin < begin + 2 );
    CIE_TEST_CHECK( begin < begin + 2 );
    CIE_TEST_REQUIRE_NOTHROW( begin < begin - 2 );
    CIE_TEST_CHECK( !(begin < begin - 2) );

    CIE_TEST_REQUIRE_NOTHROW( begin > begin - 2 );
    CIE_TEST_CHECK( begin > begin - 2 );
    CIE_TEST_REQUIRE_NOTHROW( begin > begin + 2 );
    CIE_TEST_CHECK( !(begin > begin + 2) );

    CIE_TEST_REQUIRE_NOTHROW( begin <= begin + 2 );
    CIE_TEST_CHECK( begin <= begin + 2 );
    CIE_TEST_REQUIRE_NOTHROW( begin <= begin - 2 );
    CIE_TEST_CHECK( !(begin <= begin - 2) );

    CIE_TEST_REQUIRE_NOTHROW( begin >= begin - 2 );
    CIE_TEST_CHECK( begin >= begin - 2 );
    CIE_TEST_REQUIRE_NOTHROW( begin >= begin + 2 );
    CIE_TEST_CHECK( !(begin >= begin + 2) );

    CIE_TEST_REQUIRE_NOTHROW( begin == begin + 0 );
    CIE_TEST_CHECK( begin == begin + 0 );
    CIE_TEST_REQUIRE_NOTHROW( begin == begin + 2 );
    CIE_TEST_CHECK( !(begin == begin + 2) );

    CIE_TEST_REQUIRE_NOTHROW( begin != begin + 2 );
    CIE_TEST_CHECK( begin != begin + 2 );
    CIE_TEST_REQUIRE_NOTHROW( begin != begin + 0 );
    CIE_TEST_CHECK( !(begin != begin + 0) );
    
}


}