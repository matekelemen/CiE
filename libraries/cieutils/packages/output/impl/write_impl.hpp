#ifndef CIE_UTILS_OUTPUT_WRITE_IMPL_HPP
#define CIE_UTILS_OUTPUT_WRITE_IMPL_HPP

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::utils {


template <concepts::STLContainer ContainerType>
inline void
write(  const ContainerType& container,
        std::ostream& stream )
requires concepts::Printable<typename ContainerType::value_type>
{
    CIE_BEGIN_EXCEPTION_TRACING

    for (const auto& value : container)
        stream << value << std::endl;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::STLContainer ContainerType>
void
write(  const ContainerType& container,
        std::ostream& stream )
requires (!concepts::Printable<typename ContainerType::value_type>)
            && concepts::STLContainer<typename ContainerType::value_type>
{
    CIE_BEGIN_EXCEPTION_TRACING

    for (const auto& subContainer : container)
    {
        for (const auto& value : subContainer)
            stream << value << ", ";
        stream << std::endl;
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::utils


#endif