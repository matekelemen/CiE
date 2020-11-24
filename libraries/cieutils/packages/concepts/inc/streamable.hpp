#ifndef CIE_UTILS_CONCEPTS_STREAMABLE_HPP
#define CIE_UTILS_CONCEPTS_STREAMABLE_HPP

// --- STL Includes ---
#include <concepts>
#include <sstream>


namespace cie::concepts {


template <class T>
concept StringStreamable
= requires ( T instance, std::stringstream stream )
{
    { stream << instance };
};


} // namespace cie::concepts


#endif