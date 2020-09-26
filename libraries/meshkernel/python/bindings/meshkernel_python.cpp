// --- External Includes ---
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

// --- Internal Includes ---
#include "meshkernel/inc/meshgenerator.hpp"


PYBIND11_MODULE( pymeshkernel, m )
{
  m.doc( ) = "mesh generation kernel"; // optional module docstring

  pybind11::class_<cie::meshkernel::TriangulationParameters>( m, "TriangulationParameters" )
    .def(pybind11::init<>( ) )
    .def(pybind11::init<double, double, double>( ) )
    .def_readwrite("edgeLength",&cie::meshkernel::TriangulationParameters::edgeLength )
    .def_readwrite("goodChopRatio",&cie::meshkernel::TriangulationParameters::goodChopRatio )
    .def_readwrite("divisionAngle",&cie::meshkernel::TriangulationParameters::divisionAngle );

  // Function cast is required as we have multiple overloads of triangulation
  m.def( "triangulate", ( cie::meshkernel::Triangulation (*)( const cie::meshkernel::Vertex2DVector&,
                                                         const std::vector<cie::meshkernel::IndexVector>&,
                                                         cie::meshkernel::TriangulationParameters ) )
         &cie::meshkernel::triangulate, "Triangulates a given polygon" );

  m.def( "triangulate", ( cie::meshkernel::Triangulation (*)( const cie::meshkernel::Vertex2DVector&,
                                                         cie::meshkernel::TriangulationParameters ) )
         &cie::meshkernel::triangulate, "Triangulates a given polygon" );
}