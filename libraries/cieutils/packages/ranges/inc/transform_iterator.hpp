#ifndef CIE_CIEUTILS_TRANSFORM_ITERATOR_HPP
#define CIE_CIEUTILS_TRANSFORM_ITERATOR_HPP

// --- Internal Includes ---
#include "../../concepts/inc/iterator_concepts.hpp"
#include "../../macros/inc/assertions.hpp"

// --- STL Includes ---
#include <iterator>
#include <iostream>

namespace cie::utils {


// Iterator wrapper that converts the the original object to a transformed one
// when dereferenced
template <  class BaseType,
            class ValueType,
            concepts::IteratorType BaseIteratorType,
            class SelfType >
class TransformIterator
{
public:
    typedef ValueType                                       value_type;
    typedef value_type&                                     reference;
    typedef value_type*                                     pointer;
    typedef typename BaseIteratorType::difference_type      difference_type;
    typedef typename BaseIteratorType::iterator_category    iterator_category;

    typedef BaseType                                        base_type;
    typedef BaseIteratorType                                base_iterator_type;
    typedef SelfType                                        self_type;
    typedef TransformIterator<
        BaseType,
        ValueType,
        BaseIteratorType,
        SelfType >                                          transform_iterator_type;

public:
    TransformIterator( base_iterator_type base )            : _base(base), _incrementFlag(false), _decrementFlag(false) {}
    inline reference operator*()                            { return update(); }

    inline self_type& operator++()                          { increment(); ++_base; return *thisPtr(); }
    inline self_type& operator--()                          { increment(); --_base; return *thisPtr(); }
    inline self_type& operator++(int)                       { increment(); _incrementFlag=true; return *thisPtr(); }
    inline self_type& operator--(int)                       { increment(); _decrementFlag=true; return *thisPtr(); }
    inline self_type& operator+=( difference_type offset )  { increment(); _base+=offset; return *thisPtr(); }
    inline self_type& operator-=( difference_type offset )  { increment(); _base-=offset; return *thisPtr(); }

    base_iterator_type& base()                              { return _base; }
    const base_iterator_type& base() const                  { return _base; } 

protected:
    virtual reference convert() = 0;
    virtual self_type* thisPtr()                            { CIE_ASSERT(false,"Abstract function call"); return nullptr; };

    base_iterator_type  _base;

private:
    void increment()    { if(_incrementFlag) {_base++; _incrementFlag=!_incrementFlag;} else if(_decrementFlag) {_base--; _decrementFlag=!_decrementFlag;} }
    reference update()  {  increment(); return convert(); }
    bool                _incrementFlag;
    bool                _decrementFlag;
};


}


// TransformIterator concept
namespace cie::concepts::detail {

template <class T>
concept TransformIterator
=   IteratorType<T>
    && requires ( T iterator )
    {
        { iterator.base() };
    };

}


// ---------------------------------------------------------
// OPERATOR OVERLOADS
// ---------------------------------------------------------

template <  cie::concepts::detail::TransformIterator TrIterator, 
            cie::concepts::Integer Integer>
TrIterator operator+( TrIterator lhs, Integer rhs )
{
    return TrIterator(lhs.base() + rhs);
}


template <  cie::concepts::detail::TransformIterator TrIterator, 
            cie::concepts::Integer Integer>
TrIterator operator-( TrIterator lhs, Integer rhs )
{
    return TrIterator(lhs.base() - rhs);
}


template <cie::concepts::detail::TransformIterator TrIterator>
bool operator<( TrIterator lhs, TrIterator rhs )
{
    return lhs.base() < rhs.base();
}


template <cie::concepts::detail::TransformIterator TrIterator>
bool operator<=( TrIterator lhs, TrIterator rhs )
{
    return lhs.base() <= rhs.base();
}


template <cie::concepts::detail::TransformIterator TrIterator>
bool operator>( TrIterator lhs, TrIterator rhs )
{
    return lhs.base() > rhs.base();
}


template <cie::concepts::detail::TransformIterator TrIterator>
bool operator>=( TrIterator lhs, TrIterator rhs )
{
    return lhs.base() >= rhs.base();
}


template <cie::concepts::detail::TransformIterator TrIterator>
bool operator==( TrIterator lhs, TrIterator rhs )
{
    return lhs.base() == rhs.base();
}


template <cie::concepts::detail::TransformIterator TrIterator>
bool operator!=( TrIterator lhs, TrIterator rhs )
{
    return lhs.base() != rhs.base();
}


#endif