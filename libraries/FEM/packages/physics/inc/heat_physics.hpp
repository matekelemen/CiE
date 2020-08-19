#ifndef CIE_FEM_PHYSICS_HEAT_PHYSICS_HPP
#define CIE_FEM_PHYSICS_HEAT_PHYSICS_HPP

// --- Internal Includes ---
#include "./abs_element_physics.hpp"


namespace cie::fem {


template <class ElementType>
class StaticLinearHeatPhysics1D : public AbsElementPhysics<ElementType>
{
public:
    using NT                        = typename AbsElementPhysics<ElementType>::NT;
    using matrix_update_function    = typename AbsElementPhysics<ElementType>::matrix_update_function;
    using vector_update_function    = typename AbsElementPhysics<ElementType>::vector_update_function;

public:
    template <class ...Args>
    StaticLinearHeatPhysics1D(  NT conductivity,
                                Args&&... args );

    virtual void integrateStiffness( matrix_update_function updateFunction );
    virtual void integrateLoad( vector_update_function updateFunction );

    NT& conductivity()          { return _conductivity; }
    NT conductivity() const     { return _conductivity; }

protected:
    NT  _conductivity;
};


} // namespace cie::fem

#include "../impl/heat_physics_impl.hpp"

#endif