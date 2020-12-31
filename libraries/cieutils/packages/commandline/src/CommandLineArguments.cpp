// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "cieutils/packages/commandline/inc/CommandLineArguments.hpp"


namespace cie::utils {


CommandLineArguments::CommandLineArguments( int argc, char const* argv[] ) :
    _keywordPrefix( "--" )
{
    CIE_BEGIN_EXCEPTION_TRACING

    std::string tmp;

    for ( Size argIndex=1; argIndex<Size(argc); ++argIndex )
    {
        tmp = argv[argIndex];

        if ( tmp.substr(0,_keywordPrefix.size()) == _keywordPrefix )   // keyword argument
        {
            auto separatorIndex = tmp.find( '=' );

            if ( separatorIndex == std::string::npos )
                CIE_THROW( Exception, "Keyword argument missing '=': " + tmp )

            if ( tmp.size() < separatorIndex+2 )
                CIE_THROW( Exception, "Keyword argument missing value after '=': " + tmp )

            if ( separatorIndex == _keywordPrefix.size() )
                CIE_THROW( Exception, "Keyword argument missing key before '=': " + tmp )
            
            auto emplacementResult = this->_kwargs.try_emplace(
                tmp.substr( _keywordPrefix.size(), separatorIndex - _keywordPrefix.size() ),
                tmp.substr( separatorIndex+1 )
            );

            if ( !emplacementResult.second )
                CIE_THROW(
                    Exception,
                    "Duplicate keyword arguments for key: " + emplacementResult.first->first
                    + "(" + emplacementResult.first->second + ", " + tmp.substr(separatorIndex+1) + ")"
                )
        }
        else                            // positional argument
            this->_args.emplace_back( tmp );
        
    }

    CIE_END_EXCEPTION_TRACING
}


CommandLineArguments& CommandLineArguments::addDefaultKeywordArgument( const std::string& r_key,
                                                                       const std::string& r_value )
{
    this->_kwargs.try_emplace(
        r_key,
        r_value
    );

    return *this;
}


const typename CommandLineArguments::argument_container&
CommandLineArguments::arguments() const
{
    return this->_args;
}


const typename CommandLineArguments::keyword_argument_container&
CommandLineArguments::keywordArguments() const
{
    return this->_kwargs;
}


typename CommandLineArguments::keyword_argument_container::const_iterator
CommandLineArguments::find( const std::string& r_key ) const
{
    auto it = this->_kwargs.find( r_key );

    if ( it == this->_kwargs.end() )
        CIE_THROW( OutOfRangeException, "Key not found: " + r_key )

    return it;
}


typename CommandLineArguments::argument_container::const_iterator
CommandLineArguments::find( Size index ) const
{
    if ( index >= this->_args.size() )
        CIE_THROW( OutOfRangeException, "Index " + std::to_string(index) + " out of range (" + std::to_string(this->_args.size()) + ")" )

    return this->_args.begin() + index;
}


} // namespace cie::utils