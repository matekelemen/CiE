#ifndef CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_HPP
#define CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_HPP

// --- Internal Includes ---
#include "../../concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::utils {


template <concepts::STLContainer ContainerType>
requires concepts::STLContainer<typename ContainerType::value_type>
class StateIncrementer
{
public:
    typedef ContainerType                               container_type;
    typedef typename ContainerType::value_type          subcontainer_type;
    typedef typename ContainerType::const_iterator      iterator_type;
    typedef typename subcontainer_type::const_iterator  subiterator_type;

    typedef std::vector<subiterator_type>               state_container;
    typedef typename state_container::iterator          state_iterator;

public:
    StateIncrementer( const container_type& container );

    void reset();

    StateIncrementer& operator++();
    StateIncrementer& operator++(int);
    const state_container& operator*() const;

protected:
    const container_type* const     _container;
    state_container                 _state;
    state_iterator                  _stateIt;
}; // class StateIncrementer


} // namespace cie::utils

#include "../impl/state_iterator_impl.hpp"

#endif