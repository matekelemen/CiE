#ifndef OPTIMIZATION_OBJECTIVE_FUNCTION_HPP
#define OPTIMIZATION_OBJECTIVE_FUNCTION_HPP

// --- Linalg Includes ---
#include "linalg/packages/types/inc/arraytypes.hpp"

// --- Internal Includes ---
#include "optimization/packages/representation/inc/element.hpp"

// --- STL Includes ---
#include <memory>
#include <functional>

namespace cie::opt {

// DataType -> ObjectiveType
template <typename DataType, typename ObjectiveType>
using ObjectivePtr = std::function<const ObjectiveType(const DataType&)>;

// R^N -> R^M
template <size_t DataSize, size_t ObjectiveSize>
using RNRMObjectivePtr = ObjectivePtr< RNRMElement<DataSize,ObjectiveSize>, DoubleArray<ObjectiveSize> >;

// R^N -> R
template <size_t N>
using RNRObjectivePtr = ObjectivePtr< RNRElement<N>, double >;


} // namespace cie::opt

#endif