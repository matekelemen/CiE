// --- External Includes ---
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

// --- Internal Includes ---
#include "CSG/packages/clustering/inc/minimumdisc.hpp"


PYBIND11_MODULE( pycsg,m ){
    m.doc() = "Constructive Solid Geometry module";

	pybind11::class_<cie::csg::MinimumEnclosingDisc>(m, "MinimumEnclosingDisc")
		.def(pybind11::init<cie::csg::PointSet2D>())
		.def("build", &cie::csg::MinimumEnclosingDisc::build)
		.def("getCenter", &cie::csg::MinimumEnclosingDisc::getCenter)
		.def("getRadius", &cie::csg::MinimumEnclosingDisc::getRadius)
		.def("getActiveIndices",&cie::csg::MinimumEnclosingDisc::getActiveIndices)
		;
}
