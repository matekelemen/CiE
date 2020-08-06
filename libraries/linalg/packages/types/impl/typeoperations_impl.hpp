#ifndef LINALG_TYPE_OPERATIONS_IMPL_HPP
#define LINALG_TYPE_OPERATIONS_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- STL Includes ---
#include <cmath>
#include <stdexcept>


namespace cie::linalg {

template <typename T>
double norm2(const T& object)
{
    double out = 0.0;
    for (size_t i=0; i<object.size(); ++i)
    {
        out += object[i] * object[i];
    }
    return out;
}


template <typename T>
double norm(const T& object)
{
    return std::sqrt(norm2<T>(object));
}


template <typename T>
void normalize(T& object, NormFunction<T> measure, double tolerance)
{
    double objectNorm = measure(object);
    if ( std::abs(objectNorm)<tolerance )
    {
        CIE_THROW( std::runtime_error, "Cannot normalize an object with 0 norm!" )
    }
    else
    {
        for (size_t i=0; i<object.size(); ++i)
        {
            object[i] /= objectNorm;
        } // for (size_t i=0; i<object.size(); ++i)
    } // else
}

} // namespace cie::linalg

#endif