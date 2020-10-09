#ifndef CIE_UTILS_STL_EXTENSION_THREADSAFE_MAP_IMPL_HPP
#define CIE_UTILS_STL_EXTENSION_THREADSAFE_MAP_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <utility>


namespace cie::mp {


/* --- ELEMENT ACCESS --- */

template <class MapType>
inline const typename ThreadSafeMap<MapType>::stored_type&
ThreadSafeMap<MapType>::at( const typename MapType::key_type& r_key )
{
    _mutex.lock();
    return MapType::at( r_key );
    _mutex.unlock();
}



template <class MapType>
inline typename ThreadSafeMap<MapType>::stored_type&
ThreadSafeMap<MapType>::operator[]( const typename MapType::key_type& r_key )
{
    _mutex.lock();
    return MapType::operator[]( r_key );
    _mutex.unlock();
}



template <class MapType>
inline typename ThreadSafeMap<MapType>::stored_type&
ThreadSafeMap<MapType>::operator[]( typename MapType::key_type&& key )
{
    _mutex.lock();
    return MapType::operator[]( std::move(key) );
    _mutex.unlock();
}



/* --- CONSTRUCTION/DESTRUCTION--- */

template <class MapType>
ThreadSafeMap<MapType>::ThreadSafeMap() :
    _mutex()
{
    CIE_THROW( NotImplementedException, "ThreadSafeMap does not work yet" );
}


/* --- MODIFIERS --- */

template <class MapType>
void
ThreadSafeMap<MapType>::clear()
{
    _mutex.lock();
    MapType::clear();
    _mutex.unlock();
}



template <class MapType>
template <class P>
inline std::pair<typename MapType::iterator,bool>
ThreadSafeMap<MapType>::insert( P&& value )
{
    _mutex.lock();
    return MapType::insert( std::move(value) );
    _mutex.unlock();
}



template <class MapType>
template <class InputIt>
inline typename MapType::iterator
ThreadSafeMap<MapType>::insert( InputIt it_first, InputIt it_last )
{
    _mutex.lock();
    return MapType::insert( it_first, it_last );
    _mutex.unlock();
}



template <class MapType>
inline std::pair<typename MapType::iterator,bool>
ThreadSafeMap<MapType>::insert( const typename MapType::value_type& r_value )
{
    _mutex.lock();
    return MapType::insert(r_value );
    _mutex.unlock();
}



template <class MapType>
inline std::pair<typename MapType::iterator,bool>
ThreadSafeMap<MapType>::insert( typename MapType::value_type&& value )
{
    _mutex.lock();
    MapType::insert( std::move(value) );
    _mutex.unlock();
}



template <class MapType>
template <class ...Args>
inline std::pair<typename MapType::iterator,bool>
ThreadSafeMap<MapType>::emplace( Args&&... args )
{
    _mutex.lock();
    return MapType::emplace( std::forward<Args>(args)... );
    _mutex.unlock();
}



template <class MapType>
template <class ...Args>
inline std::pair<typename MapType::iterator,bool>
ThreadSafeMap<MapType>::try_emplace( const typename MapType::key_type& r_key,
                                      Args&& ...args )
{
    _mutex.lock();
    return MapType::try_emplace( r_key,
                                 std::forward<Args>(args)... );
    _mutex.unlock();
}



template <class MapType>
template <class ...Args>
inline std::pair<typename MapType::iterator,bool>
ThreadSafeMap<MapType>::try_emplace( const typename MapType::key_type&& key,
                                      Args&& ...args )
{
    _mutex.lock();
    return MapType::try_emplace( std::move(key),
                                 std::forward<Args>(args)... );
    _mutex.unlock();
}



template <class MapType>
inline typename MapType::iterator
ThreadSafeMap<MapType>::erase( typename MapType::iterator it_pos )
{
    _mutex.lock();
    return MapType::erase( it_pos );
    _mutex.unlock();
}



template <class MapType>
inline typename MapType::iterator
ThreadSafeMap<MapType>::erase( typename MapType::const_iterator it_pos )
{
    _mutex.lock();
    return MapType::erase( it_pos );
    _mutex.unlock();
}



template <class MapType>
inline typename MapType::iterator
ThreadSafeMap<MapType>::erase( typename MapType::const_iterator it_first,
                                typename MapType::const_iterator it_last )
{
    _mutex.lock();
    return MapType::erase( it_first, it_last );
    _mutex.unlock();
}



template <class MapType>
inline typename MapType::size_type
ThreadSafeMap<MapType>::erase( const typename MapType::key_type& r_key )
{
    _mutex.lock();
    return MapType::erase( r_key );
    _mutex.unlock();
}



/* --- GETTERS --- */

template <class MapType>
inline typename MapType::size_type
ThreadSafeMap<MapType>::count( const typename MapType::key_type& r_key )
{
    _mutex.lock();
    return MapType::count( r_key );
    _mutex.unlock();
}



template <class MapType>
template <class KeyType>
inline typename MapType::size_type
ThreadSafeMap<MapType>::count( const KeyType& r_key )
{
    _mutex.lock();
    return MapType::count( r_key );
    _mutex.unlock();
}



template <class MapType>
inline typename MapType::iterator
ThreadSafeMap<MapType>::find( const typename MapType::key_type& r_key )
{
    _mutex.lock();
    return MapType::find( r_key );
    _mutex.unlock();
}



template <class MapType>
inline bool
ThreadSafeMap<MapType>::contains( const typename MapType::key_type& r_key )
{
    _mutex.lock();
    return MapType::contains( r_key );
    _mutex.unlock();
}



template <class MapType>
template <class KeyType>
inline bool
ThreadSafeMap<MapType>::contains( const KeyType& r_key )
{
    _mutex.lock();
    return MapType::contains( r_key );
    _mutex.unlock();
}



template <class MapType>
inline bool
ThreadSafeMap<MapType>::empty()
{
    _mutex.lock();
    return MapType::empty();
    _mutex.unlock();
}



template <class MapType>
inline typename MapType::size_type
ThreadSafeMap<MapType>::size()
{
    _mutex.lock();
    return MapType::size();
    _mutex.unlock();
}



template <class MapType>
inline typename MapType::iterator
ThreadSafeMap<MapType>::begin()
{
    _mutex.lock();
    return MapType::begin();
    _mutex.unlock();
}



template <class MapType>
inline typename MapType::iterator
ThreadSafeMap<MapType>::end()
{
    _mutex.lock();
    return MapType::end();
    _mutex.unlock();
}


} // namespace cie::mp


#endif