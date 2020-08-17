#ifndef CIE_FEM_PHYSICS_HEAT_PHYSICS_HPP
#define CIE_FEM_PHYSICS_HEAT_PHYSICS_HPP

// --- Internal Includes ---
#include "./abs_element_physics.hpp"


namespace cie::fem {


template <class ElementType>
class StaticLinearHeatPhysics : AbsElementPhysics<ElementType>
{
public:
    using NT = typename AbsElementPhysics<ElementType>::NT;

public:
    StaticLinearHeatPhysics( NT conductivity );
    NT& conductivity()          { return _conductivity; }
    NT conductivity() const     { return _conductivity; }

protected:
    NT  _conductivity;
};


} // namespace cie::fem

#include "../impl/heat_physics_impl.hpp"

#endif