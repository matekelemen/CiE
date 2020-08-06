#ifndef CIE_UTILITIES_CACHE_IMPL_HPP
#define CIE_UTILITIES_CACHE_IMPL_HPP

// --- Internal Includes ---
#include "../../exceptions/inc/exception.hpp"


namespace cie::utils
{


template <  concepts::STLContainer InputContainer,
            concepts::STLContainer OutputContainer >
inline bool
Cache<InputContainer,OutputContainer>::insert(  const InputContainer& input,
                                                const OutputContainer& output )
{
    auto id = this->hash( input.begin(), input.end() );
    return _cache.emplace( id, output ).second;
}


template <  concepts::STLContainer InputContainer,
            concepts::STLContainer OutputContainer >
inline const OutputContainer&
Cache<InputContainer,OutputContainer>::operator[]( const InputContainer& input ) const
{
    auto id     = this->hash( input.begin(), input.end() );
    return this->operator[](id);
}


template <  concepts::STLContainer InputContainer,
            concepts::STLContainer OutputContainer >
inline const OutputContainer&
Cache<InputContainer,OutputContainer>::operator[]( Size cacheID ) const
{
    auto mapIt  = _cache.find(cacheID);
    if (mapIt == _cache.end())
        throw OutOfRangeException( "Cache::operator[]" );
    
    return mapIt->second;
}


template <  concepts::STLContainer InputContainer,
            concepts::STLContainer OutputContainer >
inline Size
Cache<InputContainer,OutputContainer>::hash(    typename Cache<InputContainer,OutputContainer>::input_iterator begin,
                                                typename Cache<InputContainer,OutputContainer>::input_iterator end ) const
{
    Size seed = Size(std::distance(begin,end));
    for ( ; begin!=end; begin++ )
        seed ^= Size(&(*begin)) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    return seed;
}


}


#endif