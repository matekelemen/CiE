#ifndef CIE_UTILS_STL_EXTENSION_THREADSAFE_MAP_HPP
#define CIE_UTILS_STL_EXTENSION_THREADSAFE_MAP_HPP

// --- Internal Includes ---
#include "cieutils/packages/concurrency/inc/OpenMPMutex.hpp"


namespace cie::mp {


template <class MapType>
class ThreadSafeMap : public MapType
{
public:
    using map_type      = MapType;
    using stored_type   = typename MapType::value_type::second_type;
    using mutex_type    = OpenMPMutex;

public:

    /* --- CONSTRUCTION/DESTRUCTION --- */

    ThreadSafeMap();

    /* --- ELEMENT ACCESS --- */

    const stored_type& at( const typename MapType::key_type& r_key );
    stored_type& operator[]( const typename MapType::key_type& r_key );
    stored_type& operator[]( typename MapType::key_type&& key );

    /* --- MODIFIERS --- */

    void clear();

    std::pair<typename MapType::iterator,bool> insert( const typename MapType::value_type& r_value );
    std::pair<typename MapType::iterator,bool> insert( typename MapType::value_type&& value );

    typename MapType::iterator erase( typename MapType::iterator it_pos );
    typename MapType::iterator erase( typename MapType::const_iterator it_pos );

    typename MapType::iterator erase( typename MapType::const_iterator it_first,
                                      typename MapType::const_iterator it_last );

    typename MapType::size_type erase( const typename MapType::key_type& r_key );

    template <class P>
    std::pair<typename MapType::iterator,bool> insert( P&& value );

    template <class InputIt>
    typename MapType::iterator insert( InputIt it_first, InputIt it_last );

    template <class ...Args>
    std::pair<typename MapType::iterator,bool> emplace( Args&&... args );

    template <class ...Args>
    std::pair<typename MapType::iterator,bool> try_emplace( const typename MapType::key_type& r_key,
                                                            Args&& ...args );

    template <class ...Args>
    std::pair<typename MapType::iterator,bool> try_emplace( const typename MapType::key_type&& key,
                                                            Args&& ...args );

    // TODO: add all modifier functions and their overloads

    /* --- GETTERS --- */

    typename MapType::size_type count( const typename MapType::key_type& r_key );

    template <class KeyType>
    typename MapType::size_type count( const KeyType& r_key );

    typename MapType::iterator find( const typename MapType::key_type& r_key );

    bool contains( const typename MapType::key_type& r_key );

    template <class KeyType>
    bool contains( const KeyType& r_key );

    bool empty();

    typename MapType::size_type size();

    typename MapType::iterator begin();

    typename MapType::iterator end();

protected:
    mutex_type _mutex;
};


} // namespace cie::mp

#include "cieutils/packages/concurrency/impl/ThreadSafeMap_impl.hpp"

#endif