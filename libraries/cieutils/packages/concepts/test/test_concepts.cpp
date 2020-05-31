// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/container_concepts.hpp"
#include "../inc/iterator_concepts.hpp"
#include "../inc/basic_concepts.hpp"

// --- STD Includes ---
#include <vector>
#include <array>
#include <deque>
#include <map>
#include <unordered_map>
#include <utility>

namespace cie {


template <concepts::NumericType T>
void numericTest( T value )
{}

template <concepts::PointerType T>
void pointerTest( T ptr )
{}

template <class ValueType, class PointerType>
void classPointerTest( PointerType ptr )
requires concepts::ClassPointer<PointerType,ValueType>
{}

template <concepts::NonPointerType T>
void nonPointerTest( T value )
{}

template <concepts::IteratorType IteratorType>
void iteratorTest( IteratorType it )
{}

template <class ValueType, class IteratorType>
void classIteratorTest( IteratorType it )
requires concepts::ClassIterator<IteratorType, ValueType>
{}

template <concepts::STLContainer ContainerType>
void containerTest( ContainerType container )
{}

template <class ValueType, class ContainerType>
void classContainerTest( ContainerType container )
requires concepts::ClassContainer<ContainerType,ValueType>
{}

template <concepts::PointerContainer ContainerType>
void pointerContainerTest( ContainerType container )
{}

template <concepts::NonPointerContainer ContainerType>
void nonPointerContainerTest( ContainerType container )
{}

template <concepts::IteratorContainer ContainerType>
void iteratorContainerTest( ContainerType container )
{}




TEST_CASE( "Numeric Concepts", "[concepts]" )
{
    int     a = 0;
    uint    b = 0;
    float   c = 0;
    double  d = 0;

    numericTest(a);
    numericTest(b);
    numericTest(c);
    numericTest(d);
}


TEST_CASE( "Pointer Concepts", "[concepts]" )
{
    int     a = 0;
    void*   b = nullptr;

    pointerTest( &a );
    pointerTest( b );
    classPointerTest<int>( &a );
    nonPointerTest( a );
}


TEST_CASE( "Iterator Concepts", "[concepts]" )
{
    std::array<int,2>           a;
    std::vector<int>            b;
    std::deque<int>             c;
    std::map<int,int>           d;
    std::unordered_map<int,int> e;

    iteratorTest(a.begin());
    iteratorTest(b.begin());
    iteratorTest(c.begin());
    iteratorTest(d.begin());
    iteratorTest(e.begin());

    classIteratorTest<int>(a.begin());
    classIteratorTest<int>(b.begin());
    classIteratorTest<int>(c.begin());
    classIteratorTest<std::pair<const int,int>>(d.begin());
    classIteratorTest<std::pair<const int,int>>(e.begin());
}


TEST_CASE( "Container Concepts", "[concepts]" )
{
    std::array<int,2>                       a;
    std::vector<int>                        b;
    std::deque<int>                         c;
    std::map<int,int>                       d;
    std::unordered_map<int,int>             e;
    std::deque<int*>                        f;
    std::deque<std::deque<int>::iterator>   g;

    containerTest( a );
    containerTest( b );
    containerTest( c );
    containerTest( d );
    containerTest( e );
    containerTest( f );
    containerTest( g );
    
    classContainerTest<int>( a );
    classContainerTest<int>( b );
    classContainerTest<int>( c );
    classContainerTest<std::pair<const int,int>>( d );
    classContainerTest<std::pair<const int,int>>( e );
    classContainerTest<int*>( f );
    classContainerTest<std::deque<int>::iterator>( g );

    pointerContainerTest( f );
    iteratorContainerTest( g );

    nonPointerContainerTest( a );
    nonPointerContainerTest( b );
    nonPointerContainerTest( c );
    nonPointerContainerTest( d );
    nonPointerContainerTest( e );
    nonPointerContainerTest( g );
}


}