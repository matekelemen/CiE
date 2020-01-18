#include "../inc/ntreeutils.hpp"

namespace cie {
namespace csg {


const size_t intPow(int base, size_t exponent)
{
    size_t result = 1;
    for (size_t i = 0; i < exponent; ++i)
    {
        result *= base;
    }
    return result;
}


bool uniformSigns(const DoubleVector& data)
{
    bool result = data[0] > 0.0;
    for (auto it=data.begin()+1; it!=data.end(); ++it)
        if ( ((*it) > 0.0) != result) return false;
    return true;
}


}
}