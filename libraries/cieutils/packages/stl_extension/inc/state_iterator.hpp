#ifndef CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_HPP
#define CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_HPP

// --- Internal Includes ---
#include "../../concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <vector>
#include <iterator>


namespace cie::utils {


template <class IteratorType>
requires concepts::STLContainer<typename std::iterator_traits<IteratorType>::value_type>
class StateIterator
{
public:
    typedef IteratorType                                                iterator_type;
    typedef typename std::iterator_traits<iterator_type>::value_type    subcontainer_type;
    typedef typename subcontainer_type::const_iterator                  subiterator_type;
    typedef std::vector<subiterator_type>                               state_container;

public:
    StateIterator() = delete;
    StateIterator( const StateIterator& copy ) = delete;
    StateIterator& operator=( const StateIterator& copy ) = delete;

    StateIterator(  IteratorType begin,
                    IteratorType end );

    template <concepts::STLContainer ContainerType>
    StateIterator( const ContainerType& container )
    requires concepts::ClassContainer<ContainerType,subcontainer_type>;

    void reset();

    virtual StateIterator& operator++();
    virtual const state_container& operator*() const;

protected:
    IteratorType        _begin;
    IteratorType        _end;
    state_container     _state;
}; // class StateIterator


// ---------------------------------------------------------
// CONVENIENCE FUNCTIONS
// ---------------------------------------------------------

template <class IteratorType>
StateIterator<IteratorType>
makeStateIterator(  IteratorType begin,
                    IteratorType end )
requires concepts::STLContainer<typename std::iterator_traits<IteratorType>::value_type>;


template <concepts::STLContainer ContainerType>
StateIterator<typename ContainerType::const_iterator>
makeStateIterator( const ContainerType& container )
requires concepts::STLContainer<typename ContainerType::value_type>;


} // namespace cie::utils

#include "../impl/state_iterator_impl.hpp"

#endif