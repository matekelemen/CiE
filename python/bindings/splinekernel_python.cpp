#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
#include "pybind11/functional.h"

#include "basisfunctions.hpp"
#include "curve.hpp"
#include "surface.hpp"
#include "interpolation.hpp"
#include "finiteelements.hpp"

// This header defines how to convert between numpy array and linalg::Matrix
#include "denseMatrixConversion.hpp"

#include "sparseMatrixConversion.hpp"

PYBIND11_MODULE( pysplinekernel, m ) 
{
    m.doc( ) = "spline computation kernel"; // optional module docstring

    m.def( "evaluateBSplineBasis", &cie::splinekernel::evaluateBSplineBasis, "Evaluates single b-spline basis function." );
    m.def( "evaluate2DCurve", &cie::splinekernel::evaluate2DCurve, "Evaluates B-Spline curve by multiplying control points and basis functions." );
    m.def( "evaluate2DCurveDeBoor", &cie::splinekernel::evaluate2DCurveDeBoor, "Evaluates B-Spline using DeBoor" );
    m.def( "evaluateSurface", &cie::splinekernel::evaluateSurface, "Evaluates B-Spline surface" );
    m.def( "interpolateWithBSplineCurve", &cie::splinekernel::interpolateWithBSplineCurve,
            "Finds control points for b-spline curve that interpolates given points." );
    m.def("interpolateWithBSplineSurface", &cie::splinekernel::interpolateWithBSplineSurface,
            "Finds control points for b-spline surface that interpolates given points.");

    pybind11::class_<cie::splinekernel::BSplineFiniteElementMesh> mesh( m, "BSplineFiniteElementMesh" );

    mesh.def( pybind11::init<std::array<size_t, 2>,
                             std::array<size_t, 2>,
                             std::array<size_t, 2>,
                             std::array<double, 2>,
                             std::array<double, 2>,
                             cie::splinekernel::IntegrationPointProvider>( ) ); // constructor

    mesh.def( "boundaryDofIds", &cie::splinekernel::BSplineFiniteElementMesh::boundaryDofIds );
    mesh.def( "assembleLinearSystem", &cie::splinekernel::BSplineFiniteElementMesh::assembleLinearSystem );
    mesh.def( "knotVectors", &cie::splinekernel::BSplineFiniteElementMesh::knotVectors );

}