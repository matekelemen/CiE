#ifndef CSG_NTREENODE_HPP
#define CSG_NTREENODE_HPP

#include <stdint.h>
#include <memory>
#include "linalgtypes.hpp"
#include "ntreeutils.hpp"

namespace cie {
namespace csg {

// Define node pointer (needs forward declaration)
template <uint8_t N, uint8_t M>
class NTreeNode;

template <uint8_t N, uint8_t M>
using NTreeNodePtr = std::unique_ptr<NTreeNode<N,M>>;



template <uint8_t N, uint8_t M>
class NTreeNode {
public:
    NTreeNode(const DoubleArray<N>& center, double edgeLength);
    NTreeNode(  const NTreeNode<N, M>& parent,
                uint8_t index,
                const GeometryFunction<N>& geometry);

    bool divide(const GeometryFunction<N>& geometry, uint8_t level);

    DoubleArray<N>& center();
    const DoubleArray<N>& center() const;
    const UInt8Vector& data() const;
    const std::vector<NTreeNodePtr<N,M>>& children() const;
    const NTreeNode<N,M>& child(size_t index) const;

private:
    NTreeNode();
    void check() const;
    
    DoubleArray<N>                              _center;
    UInt8Vector                                 _data;
    std::vector<NTreeNodePtr<N,M>>              _children;
    double                                      _edgeLength;
};


}
}

#include "ntreenode_impl.hpp"

#endif