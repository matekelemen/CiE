#include "pybind11/pybind11.h"
#include "pybind11/stl.h"


namespace cie {
namespace csg {


PYBIND11_MODULE( pycsg,m ){
    m.doc() = "Constructive Solid Geometry module";
}

}
}