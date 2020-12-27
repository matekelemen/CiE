#ifndef CIE_MESH_KERNEL_MESHGENERATOR_HPP
#define CIE_MESH_KERNEL_MESHGENERATOR_HPP

// --- STL Includes ---
#include <vector>
#include <array>
#include <tuple>

namespace cie::meshkernel
{

// Define aliases (equivalent to typedefs)
using Vector2D = std::array<double, 2>;
using Vertex2D = Vector2D;
using Vertex2DVector = std::vector<Vertex2D>;

using IndexVector = std::vector<size_t>;

using TriangleConnectivity = std::array<size_t, 3>;
using Triangulation = std::pair<Vertex2DVector, std::vector<TriangleConnectivity>>;

// Aggregates triangulation quality parameters
struct TriangulationParameters
{
  double edgeLength    = 0.0;
  double goodChopRatio = 0.68;
  double divisionAngle = 3.14136 / 6.0;
};

Triangulation triangulate( const Vertex2DVector& polygon,
                           TriangulationParameters parameters );

Triangulation triangulate( const Vertex2DVector& vertices,
                           const std::vector<IndexVector>& polygonRegions,
                           TriangulationParameters parameters );

} // namespace cie::meshkernel

#endif // MESHGENERATOR_HPP
