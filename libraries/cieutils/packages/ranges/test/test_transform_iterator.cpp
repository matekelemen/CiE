// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/transform_iterator.hpp"

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



TEST_CASE( "TransformIterator", "[ranges]" )
{
    std::vector<int> base = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    typedef TransformIteratorHelper<decltype(base)::iterator> TrIterator;

    // Test constructor
    REQUIRE_NOTHROW( std::make_shared<TrIterator>(base.begin()) );
    TrIterator begin(base.begin());

    // Test dereferencing
    REQUIRE_NOTHROW( *begin );
    CHECK( *begin == char(base[0]) );

    // Test pre increment
    REQUIRE_NOTHROW( ++begin );
    CHECK( *begin == char(base[1]) );

    // Test pre decrement
    REQUIRE_NOTHROW( --begin );
    CHECK( *begin == char(base[0]) );

    // Test repeated pre increments/decremenets
    CHECK_NOTHROW( ++begin );
    CHECK_NOTHROW( ++begin );
    CHECK( *begin == char(base[2]) );

    CHECK_NOTHROW( --begin );
    CHECK_NOTHROW( --begin );
    CHECK( *begin == char(base[0]) );

    CHECK_NOTHROW( ++begin );
    CHECK_NOTHROW( --begin );
    CHECK( *begin == char(base[0]) );

    // Test post increment
    REQUIRE_NOTHROW( begin++ );
    CHECK( *begin == char(base[1]) );

    // Test post decrement
    REQUIRE_NOTHROW( begin-- );
    CHECK( *begin == char(base[0]) );

    // Test repeated post increments/decremenets
    CHECK_NOTHROW( begin++ );
    CHECK_NOTHROW( begin++ );
    CHECK( *begin == char(base[2]) );

    CHECK_NOTHROW( begin-- );
    CHECK_NOTHROW( begin-- );
    CHECK( *begin == char(base[0]) );

    CHECK_NOTHROW( begin++ );
    CHECK_NOTHROW( begin-- );
    CHECK( *begin == char(base[0]) );

    // Check member operator overloads
    REQUIRE_NOTHROW( begin += 5 );
    CHECK( *begin == char(base[5]) );
    REQUIRE_NOTHROW( begin -= 2 );
    CHECK( *begin == char(base[3]) );

    // Check operator overloads
    REQUIRE_NOTHROW( begin + 5 );
    REQUIRE_NOTHROW( *(begin + 5) );
    CHECK( *(begin + 5) == char(base[8]) );

    REQUIRE_NOTHROW( begin - 2 );
    REQUIRE_NOTHROW( *(begin - 2) );
    CHECK( *(begin - 2) == char(base[1]) );

    // Check boolean operators (non-exhaustive)
    REQUIRE_NOTHROW( begin < begin + 2 );
    CHECK( begin < begin + 2 );
    REQUIRE_NOTHROW( begin < begin - 2 );
    CHECK( !(begin < begin - 2) );

    REQUIRE_NOTHROW( begin > begin - 2 );
    CHECK( begin > begin - 2 );
    REQUIRE_NOTHROW( begin > begin + 2 );
    CHECK( !(begin > begin + 2) );

    REQUIRE_NOTHROW( begin <= begin + 2 );
    CHECK( begin <= begin + 2 );
    REQUIRE_NOTHROW( begin <= begin - 2 );
    CHECK( !(begin <= begin - 2) );

    REQUIRE_NOTHROW( begin >= begin - 2 );
    CHECK( begin >= begin - 2 );
    REQUIRE_NOTHROW( begin >= begin + 2 );
    CHECK( !(begin >= begin + 2) );

    REQUIRE_NOTHROW( begin == begin + 0 );
    CHECK( begin == begin + 0 );
    REQUIRE_NOTHROW( begin == begin + 2 );
    CHECK( !(begin == begin + 2) );

    REQUIRE_NOTHROW( begin != begin + 2 );
    CHECK( begin != begin + 2 );
    REQUIRE_NOTHROW( begin != begin + 0 );
    CHECK( !(begin != begin + 0) );
    
}


}