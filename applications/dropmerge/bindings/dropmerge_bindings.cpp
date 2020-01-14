#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "mergefunction.hpp"
#include "dynamictree.hpp"

PYBIND11_MODULE( pydropmerge, m )
{
    m.doc() = "Python bindings for specialized spacetree classes";

    pybind11::class_<cie::csg::DynamicTree>(m, "DynamicTree23")
        .def(pybind11::init<const cie::DoubleArray<N>&, double>())
        .def("divide", &cie::csg::DynamicTree::divide)
        ;
}