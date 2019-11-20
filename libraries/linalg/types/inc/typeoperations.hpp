#ifndef LINALG_TYPE_OPERATIONS_HPP
#define LINALG_TYPE_OPERATIONS_HPP

#include "functiontypes.hpp"

namespace cie {
namespace linalg {

template <typename T>
double norm2(const T& object);

template <typename T>
double norm(const T& object);

template <typename T>
void normalize(T& object, NormFunction<T> measure=&norm<T>, double tolerance=1e-15);

} // namespace linalg
}

#include "typeoperations_impl.hpp"

#endif