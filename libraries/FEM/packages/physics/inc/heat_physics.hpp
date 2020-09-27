#ifndef CIE_FEM_PHYSICS_HEAT_PHYSICS_HPP
#define CIE_FEM_PHYSICS_HEAT_PHYSICS_HPP

// --- Internal Includes ---
#include "FEM/packages/physics/inc/abs_element_physics.hpp"


namespace cie::fem {


/**
 * Interface for common functionalities of heat elements.
*/
template <class ElementType>
class AbsHeatPhysics : public AbsElementPhysics<ElementType>
{
public:
    using NT                        = AbsElementPhysics<ElementType>::NT;
    using matrix_update_function    = AbsElementPhysics<ElementType>::matrix_update_function;
    using vector_update_function    = AbsElementPhysics<ElementType>::vector_update_function;
    using coefficient_function      = std::function<NT(NT)>;
    using load_function             = std::function<NT(NT)>;

public:
    template <class ...Args>
    AbsHeatPhysics( coefficient_function conductivity,
                    load_function load,
                    Args&&... args ) :
        AbsElementPhysics<ElementType>( std::forward<Args>(args)... ),
        _conductivity(conductivity),
        _load(load)
    {}

    virtual void integrateStiffness( matrix_update_function updateFunction )        { CIE_THROW(NotImplementedException,"") }
    virtual void integrateLoad( vector_update_function updateFunction )             { CIE_THROW(NotImplementedException,"") }

    coefficient_function& conductivity()                                            { return _conductivity; }
    const coefficient_function& conductivity() const                                { return _conductivity; }
    load_function& load()                                                           { return _load; }
    const load_function& load() const                                               { return _load; }

protected:
    coefficient_function    _conductivity;
    load_function           _load;
};


template <class ElementType>
class StaticLinearHeatPhysics1D : public AbsHeatPhysics<ElementType>
{
public:
    using NT                        = typename AbsElementPhysics<ElementType>::NT;
    using matrix_update_function    = typename AbsElementPhysics<ElementType>::matrix_update_function;
    using vector_update_function    = typename AbsElementPhysics<ElementType>::vector_update_function;

public:
    template <class ...Args>
    StaticLinearHeatPhysics1D( Args&&... args ) :
        AbsHeatPhysics<ElementType>( std::forward<Args>(args)... ) 
    { CIE_STATIC_ASSERT( ElementType::dimension == 1 ) }

    virtual void integrateStiffness( matrix_update_function updateFunction ) override;
    virtual void integrateLoad( vector_update_function updateFunction ) override;
};


} // namespace cie::fem

#include "FEM/packages/physics/impl/heat_physics_impl.hpp"

#endif