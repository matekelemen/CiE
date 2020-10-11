#ifndef CIE_UTILS_NAMED_OBJECT_HPP
#define CIE_UTILS_NAMED_OBJECT_HPP

// --- STL Includes ---
#include <string>


namespace cie::utils {


class NamedObject
{
public:
    NamedObject( const std::string& r_name );
    const std::string& name() const;

protected:
    const std::string   _name;
};


} // namespace cie::utils

#endif