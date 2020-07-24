#ifndef CIE_FEM_UTILITIES_KERNEL_HPP
#define CIE_FEM_UTILITIES_KERNEL_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>


namespace cie::fem
{


template <concepts::NumericType NT>
struct Kernel
{
    typedef NT  number_type;
};


}


#endif