#ifndef CIE_FEM_ABS_ELEMENT_PHYSICS_HPP
#define CIE_FEM_ABS_ELEMENT_PHYSICS_HPP

// --- Internal Includes ---
#include "FEM/packages/numeric/inc/integration.hpp"

// --- Utility Includes ---
#include <cieutils/types.hpp>
#include <cieutils/cache.hpp>
#include <cieutils/macros.hpp>

// --- STL Includes ---
#include <memory>
#include <vector>
#include <functional>


namespace cie::fem {


// Specialized cache that can hash a container of points
template <  concepts::STLContainer NestedContainer,
            concepts::STLContainer StoredType >
requires concepts::STLContainer<typename NestedContainer::value_type>
class NestedCache : public cie::utils::ContainerCache<NestedContainer,StoredType>
{
public:
    virtual Size hash( const NestedContainer& container ) const override;
};


/** 
 * Base class derived from the specified element type that provides the necessary tools for
 * computing the components of structural objects (stiffness matrix, load vector, etc).
 * The element base is extended with an integrator and a caching system for holding
 * the values of the basis functions and their derivatives at the integration points.
 * 
 * The cache ID of the current integration points is stored by the _cacheID protected member.
 * 
 * The caches are automatically updated when the integrator changes, and the only way
 * to modify the integrator is by constructing a new one through AbsElementPhysics::setIntegrator.
*/
template <class ElementType>
class AbsElementPhysics : public ElementType
{
public:
    using element_type                      = ElementType;
    using NT                                = typename ElementType::kernel_type::number_type;
    using basis_set_container               = std::vector<typename ElementType::value_container>;
    using basis_derivative_set_container    = std::vector<typename ElementType::point_container>;
    using basis_cache                       = NestedCache<typename ElementType::point_container, basis_set_container>;
    using basis_derivative_cache            = NestedCache<typename ElementType::point_container, basis_derivative_set_container>;
    
    using integrator_interface              = AbsQuadrature<ElementType::dimension,NT>;
    using integrator_ptr                    = std::shared_ptr<integrator_interface>;
    using integrator_const_reference        = const integrator_interface&;

    using vector_update_function            = std::function<void(Size,NT)>;
    using matrix_update_function            = std::function<void(Size,Size,NT)>;

public:
    template <class ...Args>
    AbsElementPhysics( Args&&... elementArgs );

    /** 
     * Constructs a new integrator of the desired type and update the caches. 
     * Arguments are forwarded to the constructor of the integrator.
    */
    template <class IntegratorType, class ...Args>
    integrator_const_reference setIntegrator( Args&&... integratorArgs );

    /**
     * Recompute and update all caches.
    */
    virtual void recache();

    /**
     * Convenience function for accessing basis values for to the current
     * integration points.
    */
   const basis_set_container& basisValues() const                       { return _basisCache[_cacheID]; }

   /**
     * Convenience function for accessing basis derivatives for to the current
     * integration points.
    */
   const basis_derivative_set_container& basisDerivativeValues() const  { return _basisDerivativeCache[_cacheID]; }
   

    void clearCache()                                                   { _basisCache.clear(); _basisDerivativeCache.clear(); }
    integrator_const_reference integrator() const                       { return *_integratorPtr; }

    /**
     * Call operator() of the integrator directly.
    */
    template <class ...Args>
    NT integrator( Args&&... args ) const                               { return _integratorPtr->operator()(std::forward<Args>(args)...); }

private:
    integrator_ptr                  _integratorPtr;

protected:
    Size                            _cacheID;               // <-- cache ID for the current integration points
    static basis_cache              _basisCache;
    static basis_derivative_cache   _basisDerivativeCache;

}; // class AbsElementPhysics


// Init static members
template <class ElementType>
typename AbsElementPhysics<ElementType>::basis_cache 
AbsElementPhysics<ElementType>::_basisCache;

template <class ElementType>
typename AbsElementPhysics<ElementType>::basis_derivative_cache 
AbsElementPhysics<ElementType>::_basisDerivativeCache;


} // namespace cie::fem

#include "FEM/packages/physics/impl/abs_element_physics_impl.hpp"

#endif