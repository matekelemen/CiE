#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "meshgenerator.hpp"

PYBIND11_MODULE( pymeshkernel, m )
{
  m.doc( ) = "mesh generation kernel"; // optional module docstring

  pybind11::class_<meshkernel::TriangulationParameters>( m, "TriangulationParameters" )
    .def(pybind11::init<>( ) )
    .def(pybind11::init<double, double, double>( ) )
    .def_readwrite("edgeLength",&meshkernel::TriangulationParameters::edgeLength )
    .def_readwrite("goodChopRatio",&meshkernel::TriangulationParameters::goodChopRatio )
    .def_readwrite("divisionAngle",&meshkernel::TriangulationParameters::divisionAngle );

  // Function cast is required as we have multiple overloads of triangulation
  m.def( "triangulate", ( meshkernel::Triangulation (*)( const meshkernel::Vertex2DVector&,
                                                         const std::vector<meshkernel::IndexVector>&,
                                                         meshkernel::TriangulationParameters ) )
         &meshkernel::triangulate, "Triangulates a given polygon" );

  m.def( "triangulate", ( meshkernel::Triangulation (*)( const meshkernel::Vertex2DVector&,
                                                         meshkernel::TriangulationParameters ) )
         &meshkernel::triangulate, "Triangulates a given polygon" );
}
