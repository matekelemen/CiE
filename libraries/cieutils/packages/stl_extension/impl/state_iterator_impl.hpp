#ifndef CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_IMPL_HPP
#define CIE_UTILS_STL_EXTENSION_STATE_ITERATOR_IMPL_HPP

// --- STL Includes ---
#include <algorithm>


namespace cie::utils {


template <concepts::STLContainer ContainerType>
requires concepts::STLContainer<typename ContainerType::value_type>
StateIncrementer<ContainerType>::StateIncrementer( const ContainerType& container ) :
    _container(&container)
{
}


template <concepts::STLContainer ContainerType>
requires concepts::STLContainer<typename ContainerType::value_type>
inline void
StateIncrementer<ContainerType>::reset()
{
    std::fill(  _state.begin(),
                _state.end(),
                _container.begin() );
}


template <concepts::STLContainer ContainerType>
requires concepts::STLContainer<typename ContainerType::value_type>
inline StateIncrementer<ContainerType>& 
StateIncrementer<ContainerType>::operator++()
{

}





} // namespace cie::utils


#endif