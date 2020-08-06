#ifndef CIE_UTILITIES_CACHE_HPP
#define CIE_UTILITIES_CACHE_HPP

// --- Ínternal Includes ---
#include "../../types/inc/types.hpp"
#include "../../concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <unordered_map>


namespace cie::utils
{


template <  concepts::STLContainer InputContainer,
            concepts::STLContainer OutputContainer>
class Cache
{
public:
    typedef InputContainer                              input_container;
    typedef typename input_container::const_iterator    input_iterator;
    typedef typename input_container::value_type        input_type;
    typedef OutputContainer                             output_container;
    typedef typename output_container::const_iterator   output_iterator;
    typedef typename output_container::value_type       output_type;
    typedef std::unordered_map<Size,output_container>   internal_type;
    typedef typename internal_type::iterator            internal_iterator;

    bool insert(    const input_container& input,
                    const output_container& output );
    const output_container& operator[]( const input_container& input ) const;
    const output_container& operator[]( Size cacheID ) const;

    internal_iterator begin()                       { return _cache.begin(); }
    internal_iterator end()                         { return _cache.end(); }

    virtual Size hash(  input_iterator begin,
                        input_iterator end ) const;

protected:
    internal_type _cache;
};


}

#include "../impl/cache_impl.hpp"

#endif