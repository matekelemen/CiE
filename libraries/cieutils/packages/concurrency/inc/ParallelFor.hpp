#ifndef CIE_UTILS_PARALLEL_FOR_HPP
#define CIE_UTILS_PARALLEL_FOR_HPP

// --- Internal Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <functional>
#include <thread>


namespace cie::utils {


class ParallelFor
{
public:

protected:
    static const Size _numberOfCores;
};


} // namespace cie::utils


#endif