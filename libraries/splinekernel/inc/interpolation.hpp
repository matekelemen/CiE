#ifndef CIE_INTERPOLATION_HPP
#define CIE_INTERPOLATION_HPP

// --- Linalg Includes ---
#include "linalg/linalg.hpp"

// --- STD Includes ---
#include <array>
#include <vector>

namespace cie
{
namespace splinekernel
{

using ControlPoints2D = std::array<std::vector<double>, 2>;
using ControlPointsAndKnotVector = std::pair<ControlPoints2D, std::vector<double>>;

using VectorOfMatrices = std::vector<linalg::Matrix<Double>>;
using VectorPair = std::array<std::vector<double>, 2>;
using ControlPointsAndKnotVector3D = std::pair<VectorOfMatrices, VectorPair>;

ControlPointsAndKnotVector3D interpolateWithBSplineSurface(const VectorOfMatrices& interpolationPoints,
	size_t polynomialDegreeR, size_t polynomialDegreeS);

//! Returns the control points for a b-spline curve with given degree that interpolates the given points.
ControlPointsAndKnotVector interpolateWithBSplineCurve( const ControlPoints2D& interpolationPoints,
                                                        size_t polynomialDegree );

//! Computes the parameter positions for the given global interpolation points
std::vector<double> centripetalParameterPositions( const ControlPoints2D& interpolationPoints );
std::array<std::vector<double>, 2> centripetalParameterPositions(const VectorOfMatrices& interpolationPoints);

//! Computes the knot vector for the given parameter positions using the averaging technique
std::vector<double> knotVectorUsingAveraging( const std::vector<double>& parameterPositions,
                                              size_t polynomialDegree );

} // namespace splinekernel
} // namespace cie

#endif // CIE_INTERPOLATION_HPP
