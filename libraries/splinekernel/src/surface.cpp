// --- Internal Includes ---
#include "basisfunctions.hpp"
#include "surface.hpp"

namespace cie
{
namespace splinekernel
{
namespace detail
{

std::vector<std::vector<Double>> evaluateShapeFunctions( const std::vector<Double>& knotVector,
                                                         Size numberOfControlPoints ,
                                                         Size numberOfSamplePoints )
{
    std::vector<std::vector<Double>> shapeFunctionValues( numberOfSamplePoints );

    Size polynomialDegree = knotVector.size( ) - numberOfControlPoints - 1;

    Double t0 = knotVector.front( );
    Double t1 = knotVector.back( );

    for( Size iEvaluationCoordinate = 0; iEvaluationCoordinate < numberOfSamplePoints; ++iEvaluationCoordinate )
    {
        shapeFunctionValues[iEvaluationCoordinate].resize( numberOfControlPoints );

        Double t = iEvaluationCoordinate / ( numberOfSamplePoints - 1.0 ) * ( t1 - t0 ) + t0;

        for( Size iShapeFunction = 0; iShapeFunction < numberOfControlPoints; ++iShapeFunction )
        {
            Double value = evaluateBSplineBasis( t, iShapeFunction, polynomialDegree, knotVector );

            shapeFunctionValues[iEvaluationCoordinate][iShapeFunction] = value;
        }
    }

    return shapeFunctionValues;
}

Double computeComponent( const std::vector<Double>& Nr,
                         const std::vector<Double>& Ns,
                         const linalg::Matrix<Double>& controlPointValues )
{
    Size size1 = controlPointValues.size1( );
    Size size2 = controlPointValues.size2( );

    Double value = 0.0;

    // Compute sum over basis functions (tensor product) times control point value
    for( Size iCP = 0; iCP < size1; ++iCP )
    {
        for( Size jCP = 0; jCP < size2; ++jCP )
        {
            value += Nr[iCP] * Ns[jCP] * controlPointValues( iCP, jCP );
        }
    }

    return value;
}

} // splinesurfacehelper

VectorOfMatrices evaluateSurface( const std::array<std::vector<Double>, 2>& knotVectors,
                                  const VectorOfMatrices& controlPoints,
                                  std::array<Size, 2> numberOfSamplePoints )
{
    using Shapes1D = std::vector<std::vector<Double>>;

    // First evaluate shape functions separately in both coordinate directions
    Shapes1D shapesR = detail::evaluateShapeFunctions( knotVectors[0], controlPoints[0].size1( ), numberOfSamplePoints[0] );
    Shapes1D shapesS = detail::evaluateShapeFunctions( knotVectors[1], controlPoints[0].size2( ), numberOfSamplePoints[1] );

    VectorOfMatrices result( controlPoints.size( ) );

    // Loop over components, e.g. x, y and z, each being a 2D matrix of values
    for( Size iComponent = 0; iComponent < controlPoints.size( ); ++iComponent )
    {
        result[iComponent] = linalg::Matrix( numberOfSamplePoints[0], numberOfSamplePoints[1], 0.0 );

        // Loop over all sample points in local coordinates r and s
        for( Size iR = 0; iR < numberOfSamplePoints[0]; ++iR )
        {
            for( Size iS = 0; iS < numberOfSamplePoints[1]; ++iS )
            {
                // Compute tensor product and multiply by control point values
                result[iComponent]( iR, iS ) = detail::computeComponent( shapesR[iR], shapesS[iS], controlPoints[iComponent] );
            }
        }
    }

    return result;
}

} // namespace splinekernel
} // namespace cie
