#include "utilities.hpp"
#include <stdexcept>

namespace cie
{
namespace splinekernel
{

void check( bool value, const char message[] )
{
    if( !value )
    {
        throw std::runtime_error( message );
    }
}

} // splinekernel
} // cie
