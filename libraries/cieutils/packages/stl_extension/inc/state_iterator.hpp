#ifndef CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_HPP
#define CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_HPP

// --- Internal Includes ---
#include "../../concepts/inc/container_concepts.hpp"
#include "../../types/inc/types.hpp"

// --- STL Includes ---
#include <vector>
#include <iterator>
#include <functional>


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

    void reset();

    virtual StateIterator& operator++();
    virtual const state_container& operator*() const;

protected:
    IteratorType        _begin;
    IteratorType        _end;
    state_container     _state;
}; // class StateIterator





namespace detail {
template <concepts::STLContainer ContainerType>
struct container_reference_wrapper : public std::reference_wrapper<ContainerType>
{
    typedef typename ContainerType::value_type      value_type;
    typedef typename ContainerType::size_type       size_type;
    typedef typename ContainerType::difference_type difference_type;
    typedef typename ContainerType::iterator        iterator;
    typedef typename ContainerType::const_iterator  const_iterator;

    container_reference_wrapper( ContainerType& container ) 
        : std::reference_wrapper<ContainerType>(container)
    {}

    Size size() const                   { return this->get().size(); }
    iterator begin()                    { return this->get().begin(); }
    const_iterator begin() const        { return this->get().begin(); }
    iterator end()                      { return this->get().end(); }
    const_iterator end() const          { return this->get().end(); }
    const_iterator cbegin() const       { return this->get().cbegin(); }
    const_iterator cend() const         { return this->get().cend(); }
}; // struct container_reference_wrapper
} // namespace detail



// Helper class for permutations
template <concepts::STLContainer ContainerType>
class InternalStateIterator : 
    public StateIterator<typename std::vector<detail::container_reference_wrapper<const ContainerType>>::const_iterator>
{
public:
    InternalStateIterator(  const ContainerType& container,
                            Size size );

protected:
    std::vector<detail::container_reference_wrapper<const ContainerType>> _dummyContainer;
};


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


/** Create a state iterator structure with 'size' components
 *  that has |container.size()|^size number of states.
 *  This basically iterator through the permutations
 *  of the container values.
 */
template <concepts::STLContainer ContainerType>
InternalStateIterator<const ContainerType>
makeInternalStateIterator(  const ContainerType& container,
                            Size size );


} // namespace cie::utils

#include "../impl/state_iterator_impl.hpp"

#endif