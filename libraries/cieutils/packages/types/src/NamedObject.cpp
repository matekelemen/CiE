// --- Internal Includes ---
#include "cieutils/packages/types/inc/NamedObject.hpp"


namespace cie::utils {


NamedObject::NamedObject( const std::string& r_name ) :
    _name( r_name )
{
}


const std::string& NamedObject::name() const
{
    return _name;
}


} // namespace cie::utils