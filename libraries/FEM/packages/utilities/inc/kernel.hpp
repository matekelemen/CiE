#ifndef CIE_FEM_UTILITIES_KERNEL_HPP
#define CIE_FEM_UTILITIES_KERNEL_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"


namespace cie::fem {


template <concepts::NumericType NT>
struct Kernel
{
    typedef NT  number_type;
};


} // namespace cie::fem


#endif