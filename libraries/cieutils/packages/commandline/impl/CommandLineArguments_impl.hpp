// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"


namespace cie::utils {


template <class ContainerType>
requires concepts::ClassContainer<ContainerType, std::pair<std::string,std::string>>
CommandLineArguments&
CommandLineArguments::addDefaultKeywordArguments( const ContainerType& r_keyValuePairs )
{
    for ( const auto& r_keyValuePair : r_keyValuePairs )
        this->addDefaultKeywordArguments(
            r_keyValuePair.first,
            r_keyValuePair.second
        );
    return *this;
}


template <class KeyContainer, class ValueContainer>
requires concepts::ClassContainer<KeyContainer, std::string>
         && concepts::ClassContainer<ValueContainer, std::string>
CommandLineArguments&
CommandLineArguments::addDefaultKeywordArguments( const KeyContainer& r_keys,
                                                  const ValueContainer& r_values )
{
    CIE_CHECK( 
        r_keys.size() == r_values.size(),
        "Number of keys ("
            + std::to_string( r_keys.size() )
            + ") and number of values ("
            + std::to_string( r_values.size() )
            + ") must match!" 
    )

    auto it_key    = r_keys.begin();
    auto it_value  = r_values.begin();
    auto it_keyEnd = r_keys.end();

    for ( ; it_key!=it_keyEnd; ++it_key,++it_value )
        this->addDefaultKeywordArgument(
            *it_key,
            *it_value
        );

    return *this;
}


/* --- std::string --- */

template <>
inline std::string
CommandLineArguments::get<std::string>( const std::string& r_key ) const
{
    return this->find( r_key )->second;
}

template <>
inline std::string
CommandLineArguments::get<std::string>( Size index ) const
{
    return *this->find( index );
}


/* --- int --- */

template <>
inline int
CommandLineArguments::get<int>( const std::string& r_key ) const
{
    return std::atoi( this->find(r_key)->second.c_str() );
}


template <>
inline int
CommandLineArguments::get<int>( Size index ) const
{
    return std::atoi( this->find(index)->c_str() );
}


/* --- Size --- */

template <>
inline Size
CommandLineArguments::get<Size>( const std::string& r_key ) const
{
    long long int value = std::atoi( this->find(r_key)->second.c_str() );

    if ( value < 0 )
        CIE_THROW( Exception, "Instructed to convert negative integer to unsigned type" )

    return value;
}


template <>
inline Size
CommandLineArguments::get<Size>( Size index ) const
{
    long long int value = std::atoi( this->find(index)->c_str() );

    if ( value < 0 )
        CIE_THROW( Exception, "Instructed to convert negative integer to unsigned type" )

    return value;
}


/* --- float --- */

template <>
inline float
CommandLineArguments::get<float>( const std::string& r_key ) const
{
    return std::atof( this->find(r_key)->second.c_str() );
}


template <>
inline float
CommandLineArguments::get<float>( Size index ) const
{
    return std::atof( this->find(index)->c_str() );
}


/* --- double --- */

template <>
inline double
CommandLineArguments::get<double>( const std::string& r_key ) const
{
    return std::stod( this->find(r_key)->second );
}


template <>
inline double
CommandLineArguments::get<double>( Size index ) const
{
    return std::stod( *this->find(index) );
}



/* --- DEFAULT --- */

template <class T>
T CommandLineArguments::get( const std::string& r_key ) const
{
    CIE_THROW( Exception, "Specified type conversion not immplemented for CommandLineArgument" )
}


template <class T>
T CommandLineArguments::get( Size index ) const
{
    CIE_THROW( Exception, "Specified type conversion not immplemented for CommandLineArgument" )
}


} // namespace cie::utils