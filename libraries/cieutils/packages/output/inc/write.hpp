#ifndef CIE_UTILS_OUTPUT_WRITE_HPP
#define CIE_UTILS_OUTPUT_WRITE_HPP

// --- Internal Includes ---
#include "../../concepts/inc/container_concepts.hpp"

// --- STL Includes ---
#include <iostream>


namespace cie::concepts {
template <class T>
concept Printable
= requires ( T instance )
{
    { std::cout << instance };
};
} // namespace cie::concepts


namespace cie::utils {


/** 
 * Write contents of a container with directly
 * printable values.
*/
template <concepts::STLContainer ContainerType>
void
write(  const ContainerType& container,
        std::ostream& stream = std::cout )
requires concepts::Printable<typename ContainerType::value_type>;


/** 
 * Write contents of a nested container with directly
 * printable values.
*/
template <concepts::STLContainer ContainerType>
void
write(  const ContainerType& container,
        std::ostream& stream = std::cout )
requires (!concepts::Printable<typename ContainerType::value_type>)
            && concepts::STLContainer<typename ContainerType::value_type>;


} // namespace cie::utils

#include "../impl/write_impl.hpp"

#endif