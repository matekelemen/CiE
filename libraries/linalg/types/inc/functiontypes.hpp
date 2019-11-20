#ifndef LINALG_FUNCTION_TYPES_HPP
#define LINALG_FUNCTION_TYPES_HPP

#include <functional>

namespace cie {

template <typename Object>
using NormFunction = std::function<double(const Object&)>;

}

#endif