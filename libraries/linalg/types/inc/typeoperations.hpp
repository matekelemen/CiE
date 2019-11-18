#ifndef LINALG_TYPE_OPERATIONS_HPP
#define LINALG_TYPE_OPERATIONS_HPP

#include "linalg/types/inc/functiontypes.hpp"

namespace linalg {

template <typename T>
double norm2(const T& object);

template <typename T>
void normalize(T& object, NormFunction<T> measure=&norm2<T>);

} // namespace linalg

#include "linalg/types/inc/typeoperations_impl.hpp"

#endif