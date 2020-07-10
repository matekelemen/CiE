#ifndef MESHKERNEL_MESH_HPP
#define MESHKERNEL_MESH_HPP

// --- Linalg Includes ---
#include "linalg/types.hpp"

// --- STL Includes ---
#include <memory>

namespace cie {
namespace meshkernel {


using VertexGrid        = std::array<linalg::Matrix, 3>;
using TriangleVector    = std::vector<UIntArray<3>>;

using VertexGridPtr     = std::unique_ptr<VertexGrid>;
using TriangleVectorPtr = std::unique_ptr<TriangleVector>;


class TriangleMesh 
{
public:
    TriangleMesh() = delete;
    explicit TriangleMesh(  VertexGrid& vertices,
                            TriangleVector& faces );    

private:
    VertexGridPtr       _vertices;
    TriangleVectorPtr   _faces;

};

}
}

#endif