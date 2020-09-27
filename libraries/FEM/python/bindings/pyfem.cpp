// --- External Includes ---
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"
#include "pybind11/functional.h"

// --- Internal Includes ---
#include "../../packages/physics/inc/heat_physics.hpp"
#include "../../packages/discretization/inc/abs_element.hpp"
#include "../../packages/numeric/inc/basisfunctions.hpp"

#include "./convenience_functions.hpp"
#include "./material_functions.hpp"


// ---------------------------------------------------------
// TEMPLATE INSTANTIATIONS
// ---------------------------------------------------------

// General
using Double                            = cie::Double;

// Basis sets
using LinearBasisSet1D                 = cie::fem::LinearBasisFunctionSet<1,Double>;

// Element bases
using LinearElementBase1D               = cie::fem::AbsElement1D<LinearBasisSet1D>;

// Element types
using StaticLinearHeatElement1DLinear   = cie::fem::StaticLinearHeatPhysics1D<LinearElementBase1D>;



// ---------------------------------------------------------
// MODULE DEFINITION
// ---------------------------------------------------------
PYBIND11_MODULE( pyfem, module )
{
    /* --- ELEMENT TYPES --- */

    // Static linear heat element in 1D with linear basis functions
    pybind11::class_<StaticLinearHeatElement1DLinear> staticLinearHeatElement1DLinear( module, "StaticLinearHeatElement1DLinear" );
    staticLinearHeatElement1DLinear
        .def( pybind11::init<   typename StaticLinearHeatElement1DLinear::coefficient_function,
                                typename StaticLinearHeatElement1DLinear::load_function,
                                std::pair<  typename StaticLinearHeatElement1DLinear::NT,
                                            typename StaticLinearHeatElement1DLinear::NT >,
                                typename StaticLinearHeatElement1DLinear::dof_container>() )
        .def( "integrateStiffness", &StaticLinearHeatElement1DLinear::integrateStiffness )
        .def( "integrateLoad", &StaticLinearHeatElement1DLinear::integrateLoad )
        ;

    /* --- CONVENIENCE FUNCTIONS --- */
    pybind11::class_<cie::fem::ElementContainer<StaticLinearHeatElement1DLinear>> staticLinearHeatElement1DLinear_List( module, "StaticLinearHeatElement1DLinear_List" );
    staticLinearHeatElement1DLinear_List
        .def( pybind11::init<>() )
        .def( "emplace_back", &cie::fem::ElementContainer<StaticLinearHeatElement1DLinear>::emplace_back<   typename StaticLinearHeatElement1DLinear::coefficient_function,
                                                                                                            typename StaticLinearHeatElement1DLinear::load_function,
                                                                                                            std::pair<  typename StaticLinearHeatElement1DLinear::NT,
                                                                                                                        typename StaticLinearHeatElement1DLinear::NT >,
                                                                                                            typename StaticLinearHeatElement1DLinear::dof_container> )
        ;
    
    module.def( "integrateStiffness", &cie::fem::integrateStiffness<StaticLinearHeatElement1DLinear> );
    module.def( "integrateLoad", &cie::fem::integrateLoad<StaticLinearHeatElement1DLinear> );

    /* --- MATERIAL FUNCTIONS --- */
    /**
     * Function objects passed from python can't be evaluated in parallel (at least not with OpenMP)
     * and cause the process to seemingly lock up in an unresponsive state. As painful as it is, the
     * solution is to write the functions themselves in C++ and wrap specialized generators for each
     * type of desired function.
    */
   
    pybind11::class_<cie::fem::utils::ScalarMap<double>> scalarMap( module, "ScalarMap" );
    scalarMap
        .def( pybind11::init<std::function<double(double)>>() )
        .def( "__call__", &cie::fem::utils::ScalarMap<double>::operator() )
        .def( "get", &cie::fem::utils::ScalarMap<double>::get )
        ;

    module.def( "makeConstantFunction", &cie::fem::makeConstantFunction<double> );
                                                                                                


} // PYBIND11_MODULE pyfem