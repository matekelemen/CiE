#ifndef CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_IMPL_HPP
#define CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_IMPL_HPP

// --- Internal Includes ---
#include "../../concepts/inc/utility.hpp"

// --- STL Includes ---
#include <algorithm>
#include <iostream>


namespace cie::utils {


template <concepts::STLContainer ContainerType>
requires concepts::STLContainer<typename ContainerType::value_type>
StateIterator<ContainerType>::StateIterator( const ContainerType& container ) :
    _container(&container)
{
    this->reset();
}


template <concepts::STLContainer ContainerType>
requires concepts::STLContainer<typename ContainerType::value_type>
inline void
StateIterator<ContainerType>::reset()
{
    utils::setContainerSize( _state, _container->size() );
    std::transform( _container->begin(),
                    _container->end(),
                    _state.begin(),
                    [](const auto& container) { return container.begin(); } );
}


template <concepts::STLContainer ContainerType>
requires concepts::STLContainer<typename ContainerType::value_type>
inline StateIterator<ContainerType>& 
StateIterator<ContainerType>::operator++()
{
    assert( _state.size() == _container->size() );

    auto stateIt        = _state.begin();
    auto subContainerIt = _container->begin();

    while( subContainerIt != _container->end() )
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


template <concepts::STLContainer ContainerType>
requires concepts::STLContainer<typename ContainerType::value_type>
inline const typename StateIterator<ContainerType>::state_container&
StateIterator<ContainerType>::operator*() const
{
    return _state;
}





} // namespace cie::utils


#endif