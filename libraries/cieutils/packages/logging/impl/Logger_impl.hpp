#ifndef CIE_UTILS_LOGGER_IMPL_HPP
#define CIE_UTILS_LOGGER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <tuple>
#include <sstream>


namespace cie::utils {


template <class ...Args>
Logger& Logger::logs( Args&&... args )
{
    auto argTuple = std::make_tuple( std::forward<Args>(args)... );

    std::stringstream stream;
    std::apply(
        [&]( auto&&... arguments ) { ((stream << arguments),...); },
        argTuple
    );

    this->log( stream.str() );

    return *this;
}


template <class ExceptionType>
Logger& Logger::error( const std::string& r_message )
{
    log( "ERROR: " + r_message );
    CIE_THROW( ExceptionType, r_message );
    return *this;
}



/* --- LOG STREAM OVERLOADS --- */

template <concepts::Log LogType>
inline LogType&
operator<<( LogType& r_log,
            const std::string& r_message )
{
    r_log.log( r_message );
    return r_log;
}


template <concepts::Log LogType, class ContainerType>
requires concepts::STLContainer<ContainerType>
         && concepts::StringStreamable<typename ContainerType::value_type>
inline LogType&
operator<<( LogType& r_log,
            const ContainerType& r_messageContainer )
{
    std::stringstream stream;
    for ( const auto& r_message : r_messageContainer )
        stream << r_message << ' ';
    
    return r_log << stream.str();
}


template <concepts::Log LogType, concepts::StringStreamable MessageType>
inline LogType&
operator<<( LogType& r_log,
            const MessageType& r_message )
{
    std::stringstream stream;
    stream << r_message;
    return r_log << stream.str();
}


} // namespace cie::utils


#endif