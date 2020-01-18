#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "mergefunction.hpp"
#include "dynamictree.hpp"
#include "collectnodes.hpp"

// const size_t N
// const size_t M
// both defined in dynamictree.hpp

PYBIND11_MODULE( pydropmerge, m )
{
    m.doc() = "Python bindings for specialized spacetree classes";

    pybind11::class_<cie::csg::DynamicTree>(m, "DynamicTree")
        .def(pybind11::init<const cie::DoubleArray<N>&, double>())
        .def("divide", &cie::csg::DynamicTree::divide)
        .def("offset", &cie::csg::DynamicTree::offset)
        ;


    pybind11::class_<cie::csg::TreeData>(m,"TreeData")
        .def(pybind11::init<>())
        .def("centers", &cie::csg::TreeData::centers)
        .def("edgeLengths", &cie::csg::TreeData::edgeLengths)
        .def("boundaries", &cie::csg::TreeData::boundaries)
        ;


    m.def("collectNodes", &cie::csg::collectNodes, "Collect DynamicTree nodes");
}