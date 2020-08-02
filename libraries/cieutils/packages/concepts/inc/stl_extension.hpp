#ifndef CIE_UTILS_CONCEPTS_STL_EXTENSION_HPP
#define CIE_UTILS_CONCEPTS_STL_EXTENSION_HPP

// --- Internal Includes ---
#include "./iterator_concepts.hpp"
#include "./container_concepts.hpp"

// --- STL Includes ---
#include <iterator>


namespace cie::utils
{


template <concepts::STLContainer ContainerType>
class back_insert_iterator : public std::back_insert_iterator<ContainerType>
{
public:
    typedef typename ContainerType::value_type  value_type;

public:
    template <class ...Args>
    back_insert_iterator( Args&&... args ) : 
        std::back_insert_iterator<ContainerType>( std::forward<Args>(args)... ) {}

    value_type& operator*() 
        { this->container->emplace_back(); return *(this->container->end()-1); }
};


template <concepts::STLContainer ContainerType>
back_insert_iterator<ContainerType> back_inserter( ContainerType& container ) { return back_insert_iterator<ContainerType>(container); }


} // namespace cie::utils

#endif