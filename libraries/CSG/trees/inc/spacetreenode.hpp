#ifndef CSG_NTREENODE_HPP
#define CSG_NTREENODE_HPP

// --- Internal Includes ---
#include "indexconverter.hpp"
#include "spacetreeutils.hpp"
#include "linalgtypes.hpp"

// --- STD Includes ---
#include <stdint.h>
#include <memory>

namespace cie {
namespace csg {

// Define node pointer (needs forward declaration)
template <size_t N, size_t M>
class SpaceTreeNode;

template <size_t N, size_t M>
using SpaceTreeNodePtr = std::unique_ptr<SpaceTreeNode<N,M>>;



template <size_t N, size_t M>
class SpaceTreeNode {
public:
    SpaceTreeNode();
    SpaceTreeNode(const DoubleArray<N>& center, double edgeLength);
    SpaceTreeNode(  const SpaceTreeNode<N, M>& parent,
                size_t index,
                const GeometryFunction<N>& geometry);

    void evaluate(const GeometryFunction<N>& geometry, size_t index);
    void evaluate(const GeometryFunction<N>& geometry);
    
    bool divide(const GeometryFunction<N>& geometry, size_t level);

    void write(std::ostream& file) const;
    void wipe();

    DoubleArray<N> pointCoordinates(size_t index) const;
    DoubleArray<N> pointCoordinates(const UIntArray<N>& indexN) const;

    DoubleArray<N>& center();
    const DoubleArray<N>& center() const;
    const DoubleVector& data() const;
    const std::vector<SpaceTreeNodePtr<N,M>>& children() const;
    const SpaceTreeNode<N,M>& child(size_t index) const;
    double edgeLength() const;

protected:
    void check() const;
    bool divideRecursive(const GeometryFunction<N>& geometry, size_t level);
    
    DoubleArray<N>                              _center;
    DoubleVector                                _data;
    std::vector<SpaceTreeNodePtr<N,M>>          _children;
    double                                      _edgeLength;
    static SpaceTreeIndexConverter<N,M>         _dataIndex;
    static SpaceTreeIndexConverter<N,2>         _centerIndex;
};


}
}

#include "spacetreenode_impl.hpp"

#endif