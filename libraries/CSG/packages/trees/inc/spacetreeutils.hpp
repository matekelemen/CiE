#ifndef CSG_NTREEUTILS_HPP
#define CSG_NTREEUTILS_HPP

// --- Linalg Includes ---
#include "linalg/types.hpp"

// --- STL Includes ---
#include <stdint.h>
#include <functional>
#include <fstream>


namespace cie::csg {


template <Size N, Size M>
class SpaceTreeNode;


template <Size N>
using GeometryFunction = std::function<Double(const DoubleArray<N>&)>;


// Integer power and base conversions --------------------------------

constexpr const Size intPow(int base, Size exponent);


template <Size M>
constexpr void baseN(Size base_10, Size base, UIntArray<M>& base_N);


template <Size M>
constexpr UIntArray<M> baseN(Size base_10, Size base);


template <Size M>
constexpr Size base10(const UIntArray<M>& base_N, Size base);


template <Size M>
constexpr Size reinterpretBase( const UIntArray<M>& baseN, Size newBase );


// Writing to file ---------------------------------------------------
template <Size N, Size M>
void writeSpaceTree(const SpaceTreeNode<N,M>& node, std::ostream& file);


template <Size N, Size M>
void writeSpaceTree(const SpaceTreeNode<N,M>& node, const std::string& filename);


// Collecting nodes --------------------------------------------------
template <class ContainerType>
bool uniformSigns(const ContainerType& data);

template <Size N, Size M>
std::vector<const SpaceTreeNode<N,M>*> boundaryNodes(const SpaceTreeNode<N,M>& root);

template <Size N, Size M>
void boundaryNodes( const SpaceTreeNode<N,M>& root,
                    std::vector<const SpaceTreeNode<N,M>*>& nodes );


}

#include "spacetreeutils_impl.hpp"

#endif