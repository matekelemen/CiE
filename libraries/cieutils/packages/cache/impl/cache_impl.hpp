#ifndef CIE_UTILITIES_CACHE_IMPL_HPP
#define CIE_UTILITIES_CACHE_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- Internal Includes ---
#include "../../exceptions/inc/exception.hpp"


namespace cie::utils {


template <  class InputType,
            class StoredType >
inline typename AbsCache<InputType,StoredType>::internal_iterator
AbsCache<InputType,StoredType>::insert( const InputType& input,
                                        typename AbsCache::generator_function generator )
{
    // Hash input and check if it's inserted
    Size id     = this->hash(input);
    auto mapIt  = _map.find(id);

    if (mapIt == _map.end())
        mapIt = _map.emplace( id, generator(input) ).first;

    return mapIt;
}


template <  class InputType,
            class StoredType >
inline const typename AbsCache<InputType,StoredType>::stored_type&
AbsCache<InputType,StoredType>::operator[]( Size inputID ) const
{
    auto mapIt = _map.find(inputID);
    if (mapIt == _map.end())
    {
        CIE_THROW(
            OutOfRangeException,
            "Input with ID " + std::to_string(inputID) + " does not have a recorded value in the cache"
        )
    }
    return mapIt->second;
}


template <  class InputType,
            class StoredType >
inline const typename AbsCache<InputType,StoredType>::stored_type&
AbsCache<InputType,StoredType>::operator[]( const InputType& input ) const
{
    return this->operator[]( this->hash(input) );
}


template <  concepts::STLContainer InputContainer,
            concepts::STLContainer OutputContainer >
inline Size
ContainerCache<InputContainer,OutputContainer>::hash( const InputContainer& container ) const
{
    Size seed = container.size();
    for ( auto it=container.begin(); it!=container.end(); it++ )
        seed ^= Size(&(*it)) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    return seed;
}


} // namespace cie::utils


#endif