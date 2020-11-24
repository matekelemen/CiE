#ifndef CIE_CIEUTILS_LOGGEE_IMPL_HPP
#define CIE_CIEUTILS_LOGGEE_IMPL_HPP

// --- STL Includes ---
#include <sstream>


namespace cie::utils {


inline Loggee&
operator<<( Loggee& r_loggee,
            const std::string& r_message )
{
    r_loggee.log( r_message );
    return r_loggee;
}


template <class ContainerType>
requires concepts::STLContainer<ContainerType>
         && concepts::StringStreamable<typename ContainerType::value_type>
inline Loggee&
operator<<( Loggee& r_loggee,
            const ContainerType& r_messageContainer )
{
    std::stringstream stream;
    for ( const auto& r_message : r_messageContainer )
        stream << r_message << ' ';
    
    return r_loggee << stream.str();
}


template <concepts::StringStreamable MessageType>
inline Loggee&
operator<<( Loggee& r_loggee,
            const MessageType& r_message )
{
    std::stringstream stream;
    stream << r_message;
    return r_loggee << stream.str();
}


} // namespace cie::utils


#endif