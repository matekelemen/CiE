#ifndef CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_IMPL_HPP
#define CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_IMPL_HPP

// --- Internal Includes ---
#include "../../concepts/inc/utility.hpp"

// --- STL Includes ---
#include <algorithm>
#include <iostream>


namespace cie::utils {


template <class IteratorType>
requires concepts::STLContainer<typename std::iterator_traits<IteratorType>::value_type>
StateIterator<IteratorType>::StateIterator( IteratorType begin,
                                            IteratorType end ) :
    _begin(begin),
    _end(end)
{
    this->reset();
}


template <class IteratorType>
requires concepts::STLContainer<typename std::iterator_traits<IteratorType>::value_type>
template <concepts::STLContainer ContainerType>
StateIterator<IteratorType>::StateIterator( const ContainerType& container )
requires concepts::ClassContainer<ContainerType,subcontainer_type> :
    StateIterator( container.begin(), container.end() )
{
}


template <class IteratorType>
requires concepts::STLContainer<typename std::iterator_traits<IteratorType>::value_type>
inline void
StateIterator<IteratorType>::reset()
{
    utils::setContainerSize( _state, std::distance(_begin,_end) );
    std::transform( _begin,
                    _end,
                    _state.begin(),
                    [](const auto& container) -> subiterator_type
                    { return container.begin(); } );
}


template <class IteratorType>
requires concepts::STLContainer<typename std::iterator_traits<IteratorType>::value_type>
inline StateIterator<IteratorType>& 
StateIterator<IteratorType>::operator++()
{
    assert( _state.size() == std::distance(_begin,_end) );

    auto stateIt        = _state.begin();
    auto subContainerIt = _begin;

    while( subContainerIt != _end )
    {
        if ( (*stateIt)+1 != subContainerIt->end() )    // Found the place to increment
        {
            ++(*stateIt);
            break;
        }
        else                                            // Overflow
        {
            *stateIt = subContainerIt->begin();
            stateIt++;
            subContainerIt++;
        }
    } // while incrementable

    return *this;
}


template <class IteratorType>
requires concepts::STLContainer<typename std::iterator_traits<IteratorType>::value_type>
inline const typename StateIterator<IteratorType>::state_container&
StateIterator<IteratorType>::operator*() const
{
    return _state;
}



// ---------------------------------------------------------
// CONVENIENCE FUNCTIONS
// ---------------------------------------------------------

template <class IteratorType>
StateIterator<IteratorType>
makeStateIterator(  IteratorType begin,
                    IteratorType end )
requires concepts::STLContainer<typename std::iterator_traits<IteratorType>::value_type>
{
    return StateIterator<IteratorType>(begin,end);
}


template <concepts::STLContainer ContainerType>
StateIterator<typename ContainerType::const_iterator>
makeStateIterator( const ContainerType& container )
requires concepts::STLContainer<typename ContainerType::value_type>
{
    return StateIterator<typename ContainerType::const_iterator>( container );
}





} // namespace cie::utils


#endif