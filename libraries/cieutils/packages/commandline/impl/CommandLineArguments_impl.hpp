// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::utils {


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