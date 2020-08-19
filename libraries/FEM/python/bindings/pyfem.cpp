// --- External Includes ---
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
#include "pybind11/functional.h"

// --- Internal Includes ---
#include "../../packages/physics/inc/heat_physics.hpp"
#include "../../packages/discretization/inc/abs_element.hpp"
#include "../../packages/numeric/inc/ansatzfunctions.hpp"

#include "./convenience_functions.hpp"


// ---------------------------------------------------------
// TEMPLATE INSTANTIATIONS
// ---------------------------------------------------------

// General
using Double                            = cie::Double;

// Ansatz sets
using LinearAnsatzSet1D                 = cie::fem::LinearAnsatzFunctionSet<1,Double>;

// Element bases
using LinearElementBase1D               = cie::fem::AbsElement1D<LinearAnsatzSet1D>;

// Element types
using StaticLinearHeatElement1DLinear   = cie::fem::StaticLinearHeatPhysics1D<LinearElementBase1D>;



// ---------------------------------------------------------
// MODULE DEFINITION
// ---------------------------------------------------------
PYBIND11_MODULE( pyfem, module )
{
    /* --- ELEMENT TYPES --- */
    pybind11::class_<StaticLinearHeatElement1DLinear> staticLinearHeatElement1DLinear( module, "StaticLinearHeatElement1DLinear" );
    staticLinearHeatElement1DLinear.def( pybind11::init<    typename StaticLinearHeatElement1DLinear::NT,
                                                            std::pair<  typename StaticLinearHeatElement1DLinear::NT,
                                                                        typename StaticLinearHeatElement1DLinear::NT >,
                                                            typename StaticLinearHeatElement1DLinear::dof_container>() )
    .def( "integrateStiffness", &StaticLinearHeatElement1DLinear::integrateStiffness );

    /* --- CONVENIENCE FUNCTIONS --- */
    pybind11::class_<cie::fem::ElementList<StaticLinearHeatElement1DLinear>> staticLinearHeatElement1DLinear_List( module, "StaticLinearHeatElement1DLinear_List" );
    staticLinearHeatElement1DLinear_List
        .def( pybind11::init<>() )
        .def( "emplace_back", &cie::fem::ElementList<StaticLinearHeatElement1DLinear>::emplace_back<    typename StaticLinearHeatElement1DLinear::NT,
                                                                                                        std::pair<  typename StaticLinearHeatElement1DLinear::NT,
                                                                                                                    typename StaticLinearHeatElement1DLinear::NT >,
                                                                                                        typename StaticLinearHeatElement1DLinear::dof_container> );
    
    module.def( "integrateStiffness", &cie::fem::integrateStiffness<StaticLinearHeatElement1DLinear> );
                                                                                                


} // PYBIND11_MODULE pyfem