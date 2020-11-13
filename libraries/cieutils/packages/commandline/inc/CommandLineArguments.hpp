#ifndef CIE_UTILS_COMMAND_LINE_ARGUMENTS_HPP
#define CIE_UTILS_COMMAND_LINE_ARGUMENTS_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <string>
#include <deque>
#include <map>


namespace cie::utils {


/**
 * Store and access command line arguments with the following layout:
 *  - positional argumennts
 *  - keyword arguments: --key=value
 */
class CommandLineArguments
{
public:
    using argument_container         = std::deque<std::string>;
    using keyword_argument_container = std::map<std::string,std::string>;

public:
    CommandLineArguments( int argc, char const* argv[] );

    /**
     * Check whether a keyword argument exists
     * and return a converted object if it does
     * (if it can be converted to the specified type).
     */
    template <class T = std::string>
    T get( const std::string& r_key ) const;

    /**
     * Return a converted object at the specified position
     * (if it can be converted to the specified type).
     */
    template <class T = std::string>
    T get( Size index ) const;

    const argument_container& arguments() const;
    const keyword_argument_container& keywordArguments() const;

protected:
    typename keyword_argument_container::const_iterator find( const std::string& r_key ) const;
    typename argument_container::const_iterator find( Size index ) const;

private:
    const std::string _keywordPrefix;

protected:
    argument_container         _args;
    keyword_argument_container _kwargs;
};


} // namespace cie::utils

#include "cieutils/packages/commandline/impl/CommandLineArguments_impl.hpp"

#endif