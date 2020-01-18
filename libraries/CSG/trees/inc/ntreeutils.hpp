#ifndef CSG_NTREEUTILS_HPP
#define CSG_NTREEUTILS_HPP

#include <stdint.h>
#include <functional>
#include <fstream>

#include "linalgtypes.hpp"

namespace cie {
namespace csg {


template <uint8_t N, uint8_t M>
class NTreeNode;


template <uint8_t N>
using GeometryFunction = std::function<double(const DoubleArray<N>&)>;


// Integer power and base conversions --------------------------------

const size_t intPow(int base, uint8_t exponent);


template <uint8_t M>
void baseN(size_t base_10, uint8_t base, UInt8Array<M>& base_N);


template <uint8_t M>
UInt8Array<M> baseN(size_t base_10, uint8_t base);


template <uint8_t M>
size_t base10(const UInt8Array<M>& base_N, uint8_t base);


template <uint8_t M>
size_t reinterpretBase( const UInt8Array<M>& baseN, uint8_t newBase );


// Writing to file ---------------------------------------------------
template <uint8_t N, uint8_t M>
void writeNTree(const NTreeNode<N,M>& node, std::ostream& file);


template <uint8_t N, uint8_t M>
void writeNTree(const NTreeNode<N,M>& node, const std::string& filename);


// Collecting nodes --------------------------------------------------
bool uniformSigns(const DoubleVector& data);

template <uint8_t N, uint8_t M>
std::vector<const NTreeNode<N,M>*> boundaryNodes(const NTreeNode<N,M>& root);

template <uint8_t N, uint8_t M>
void boundaryNodes(   const NTreeNode<N,M>& root,
                                                    std::vector<const NTreeNode<N,M>*>& nodes );


}
}

#include "ntreeutils_impl.hpp"

#endif