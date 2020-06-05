#ifndef CSG_NTREEUTILS_HPP
#define CSG_NTREEUTILS_HPP

// --- Linalg Includes ---
#include "linalg/types.hpp"

// --- STD Includes ---
#include <stdint.h>
#include <functional>
#include <fstream>


namespace cie {
namespace csg {


template <size_t N, size_t M>
class SpaceTreeNode;


template <size_t N>
using GeometryFunction = std::function<double(const DoubleArray<N>&)>;


// Integer power and base conversions --------------------------------

constexpr const size_t intPow(int base, size_t exponent);


template <size_t M>
constexpr void baseN(size_t base_10, size_t base, UIntArray<M>& base_N);


template <size_t M>
constexpr UIntArray<M> baseN(size_t base_10, size_t base);


template <size_t M>
constexpr size_t base10(const UIntArray<M>& base_N, size_t base);


template <size_t M>
constexpr size_t reinterpretBase( const UIntArray<M>& baseN, size_t newBase );


// Writing to file ---------------------------------------------------
template <size_t N, size_t M>
void writeSpaceTree(const SpaceTreeNode<N,M>& node, std::ostream& file);


template <size_t N, size_t M>
void writeSpaceTree(const SpaceTreeNode<N,M>& node, const std::string& filename);


// Collecting nodes --------------------------------------------------
template <class ContainerType>
bool uniformSigns(const ContainerType& data);

template <size_t N, size_t M>
std::vector<const SpaceTreeNode<N,M>*> boundaryNodes(const SpaceTreeNode<N,M>& root);

template <size_t N, size_t M>
void boundaryNodes( const SpaceTreeNode<N,M>& root,
                    std::vector<const SpaceTreeNode<N,M>*>& nodes );


}
}

#include "spacetreeutils_impl.hpp"

#endif