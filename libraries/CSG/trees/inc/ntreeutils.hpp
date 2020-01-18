#ifndef CSG_NTREEUTILS_HPP
#define CSG_NTREEUTILS_HPP

#include <stdint.h>
#include <functional>
#include <fstream>

#include "linalgtypes.hpp"

namespace cie {
namespace csg {


template <size_t N, size_t M>
class NTreeNode;


template <size_t N>
using GeometryFunction = std::function<double(const DoubleArray<N>&)>;


// Integer power and base conversions --------------------------------

const size_t intPow(int base, size_t exponent);


template <size_t M>
void baseN(size_t base_10, size_t base, UIntArray<M>& base_N);


template <size_t M>
UIntArray<M> baseN(size_t base_10, size_t base);


template <size_t M>
size_t base10(const UIntArray<M>& base_N, size_t base);


template <size_t M>
size_t reinterpretBase( const UIntArray<M>& baseN, size_t newBase );


// Writing to file ---------------------------------------------------
template <size_t N, size_t M>
void writeNTree(const NTreeNode<N,M>& node, std::ostream& file);


template <size_t N, size_t M>
void writeNTree(const NTreeNode<N,M>& node, const std::string& filename);


// Collecting nodes --------------------------------------------------
bool uniformSigns(const DoubleVector& data);

template <size_t N, size_t M>
std::vector<const NTreeNode<N,M>*> boundaryNodes(const NTreeNode<N,M>& root);

template <size_t N, size_t M>
void boundaryNodes(   const NTreeNode<N,M>& root,
                                                    std::vector<const NTreeNode<N,M>*>& nodes );


}
}

#include "ntreeutils_impl.hpp"

#endif