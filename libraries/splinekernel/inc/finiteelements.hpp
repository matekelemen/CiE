#ifndef CIE_SPLINEKERNEL_FINITEELEMENTS_HPP
#define CIE_SPLINEKERNEL_FINITEELEMENTS_HPP

// --- STL Includes ---
#include <vector>
#include <tuple>
#include <array>
#include <functional>
#include <cstdint>

namespace cie::splinekernel {

using ScipyIndexType = std::int32_t;
using SparseDataStructure = std::tuple<ScipyIndexType*, ScipyIndexType*, double*, size_t>;
using LinearSystem = std::pair<SparseDataStructure, std::vector<double>>;

//! given a vector of x and a vector of y values, return a vector of function evaluations
using SpatialFunction = std::function<double( double, double)>;

using IntegrationPoints = std::array<std::vector<double>, 2>;
using IntegrationPointProvider = std::function<IntegrationPoints( size_t )>;

class BSplineFiniteElementMesh
{
public:
    BSplineFiniteElementMesh( std::array<size_t, 2> numberOfElements,
                              std::array<size_t, 2> polynomialDegrees,
                              std::array<size_t, 2> continuities,
                              std::array<double, 2> lengths,
                              std::array<double, 2> origin,
                              IntegrationPointProvider integrationPointProvider );

    std::vector<size_t> boundaryDofIds( size_t normal, bool side ) const;

    LinearSystem assembleLinearSystem( const SpatialFunction& sourceFunction ) const;

    std::array<std::vector<double>, 2> knotVectors( ) const;

private:
    std::array<size_t, 2> numberOfElements_, polynomialDegrees_, continuities_;
    std::array<double, 2> lengths_, origin_;

    std::array<std::vector<double>, 2> knotVectors_;
    std::vector<std::vector<size_t>> locationMaps_;

    IntegrationPointProvider integrationPointProvdier_;

    std::array<double, 2> mapToGlobal( std::array<double, 2> localCoordinates,
                                       std::array<size_t, 2> elementIndices ) const;

    void evaluateBasisAt( std::array<double, 2> globalCoordinates,
                          std::array<size_t, 2> elementIndices,
                          std::vector<double>& shapesTarget,
                          std::vector<double>& shapesDxTarget,
                          std::vector<double>& shapesDyTarget ) const;
};

} // namespace cie::splinekernel

#endif // CIE_FINITEELEMENTS_HPP
