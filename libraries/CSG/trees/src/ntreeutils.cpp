#include "../inc/ntreeutils.hpp"

namespace cie {
namespace csg {


const size_t intPow(int base, uint8_t exponent)
{
    size_t result = 1;
    for (uint8_t i = 0; i < exponent; ++i)
    {
        result *= base;
    }
    return result;
}


}
}