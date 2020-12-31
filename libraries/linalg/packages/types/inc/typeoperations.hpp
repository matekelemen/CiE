#ifndef LINALG_TYPE_OPERATIONS_HPP
#define LINALG_TYPE_OPERATIONS_HPP

// --- Internal Includes ---
#include "linalg/packages/types/inc/functiontypes.hpp"

namespace cie::linalg {

template <typename T>
double norm2(const T& object);

template <typename T>
double norm(const T& object);

template <typename T>
void normalize(T& object, NormFunction<T> measure=&norm<T>, double tolerance=1e-15);

} // namespace cie::linalg

#include "linalg/packages/types/impl/typeoperations_impl.hpp"

#endif