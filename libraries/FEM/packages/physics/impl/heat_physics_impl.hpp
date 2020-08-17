#ifndef CIE_FEM_PHYSICS_HEAT_PHYSICS_IMPL_HPP
#define CIE_FEM_PHYSICS_HEAT_PHYSICS_IMPL_HPP

// --- STL Includes ---
#include <iterator>


namespace cie::fem {


template <class ElementType>
StaticLinearHeatPhysics<ElementType>::StaticLinearHeatPhysics( NT conductivity ) :
    _conductivity(conductivity)
{
}



} // namespace cie::fem


#endif