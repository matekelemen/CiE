#ifndef CIE_UTILS_OUTPUT_WRITE_IMPL_HPP
#define CIE_UTILS_OUTPUT_WRITE_IMPL_HPP


namespace cie::utils {


template <concepts::STLContainer ContainerType>
inline void
write(  const ContainerType& container,
        std::ostream& stream )
requires concepts::Printable<typename ContainerType::value_type>
{
    for (const auto& value : container)
        stream << value << std::endl;
}


template <concepts::STLContainer ContainerType>
void
write(  const ContainerType& container,
        std::ostream& stream )
requires (!concepts::Printable<typename ContainerType::value_type>)
            && concepts::STLContainer<typename ContainerType::value_type>
{
    for (const auto& subContainer : container)
    {
        for (const auto& value : subContainer)
            stream << value << ", ";
        stream << std::endl;
    }
}


} // namespace cie::utils


#endif