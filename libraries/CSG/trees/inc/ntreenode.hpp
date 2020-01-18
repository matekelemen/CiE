#ifndef CSG_NTREENODE_HPP
#define CSG_NTREENODE_HPP

#include "indexconverter.hpp"
#include "ntreeutils.hpp"
#include "linalgtypes.hpp"

#include <stdint.h>
#include <memory>

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
    NTreeNode();
    NTreeNode(const DoubleArray<N>& center, double edgeLength);
    NTreeNode(  const NTreeNode<N, M>& parent,
                uint8_t index,
                const GeometryFunction<N>& geometry);

    void evaluate(const GeometryFunction<N>& geometry, size_t index);
    void evaluate(const GeometryFunction<N>& geometry);
    
    bool divide(const GeometryFunction<N>& geometry, uint8_t level);

    void write(std::ostream& file) const;
    void wipe();

    DoubleArray<N> pointCoordinates(size_t index) const;
    DoubleArray<N> pointCoordinates(const UInt8Array<N>& indexN) const;

    DoubleArray<N>& center();
    const DoubleArray<N>& center() const;
    const DoubleVector& data() const;
    const std::vector<NTreeNodePtr<N,M>>& children() const;
    const NTreeNode<N,M>& child(size_t index) const;
    double edgeLength() const;

private:
    void check() const;
    
    DoubleArray<N>                              _center;
    DoubleVector                                _data;
    std::vector<NTreeNodePtr<N,M>>              _children;
    double                                      _edgeLength;
    static NTreeIndexConverter<N,M>             _dataIndex;
    static NTreeIndexConverter<N,2>             _centerIndex;
};


}
}

#include "ntreenode_impl.hpp"

#endif