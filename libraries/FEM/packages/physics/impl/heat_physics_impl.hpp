#ifndef CIE_FEM_PHYSICS_HEAT_PHYSICS_IMPL_HPP
#define CIE_FEM_PHYSICS_HEAT_PHYSICS_IMPL_HPP

// --- STL Includes ---
#include <iterator>

namespace cie::fem {


template <class ElementType>
template <class ...Args>
StaticLinearHeatPhysics1D<ElementType>::StaticLinearHeatPhysics1D(  typename StaticLinearHeatPhysics1D::NT conductivity,
                                                                    Args&&... args ) :
    AbsElementPhysics<ElementType>( std::forward<Args>(args)... ),
    _conductivity(conductivity)
{
    CIE_STATIC_ASSERT( ElementType::dimension == 1 )
}


template <class ElementType>
void
StaticLinearHeatPhysics1D<ElementType>::integrateStiffness( typename StaticLinearHeatPhysics1D::matrix_update_function updateFunction )
{
    const auto& basisDerivatives        = this->basisDerivativeValues();
    const Size numberOfBasisFunctions   = basisDerivatives[0].size();

    std::vector<NT> integrandValues;
    integrandValues.reserve(basisDerivatives.size());                   // <-- number of integration points

    for (Size i=0; i<numberOfBasisFunctions; ++i)        
        for (Size j=i; j<numberOfBasisFunctions; ++j)
        {
            integrandValues.clear();
            for (const auto& basisDerivative : basisDerivatives)
                integrandValues.push_back(
                    this->_conductivity
                    * basisDerivative[i][0] * basisDerivative[j][0]     // dN_i * dN_j
                    * this->_invJacobian
                );
            NT value = this->integrator(integrandValues.begin());       // integrate
            updateFunction( this->_dofs[i],
                            this->_dofs[j],
                            value );                                    // update global matrix
            if ( i != j )
                updateFunction( this->_dofs[j],
                                this->_dofs[i],
                                value );                                // <-- symmetric
        }
} // void integrateStiffness


template <class ElementType>
void
StaticLinearHeatPhysics1D<ElementType>::integrateLoad( typename StaticLinearHeatPhysics1D::vector_update_function updateFunction )
{

} // void integrateLoad



} // namespace cie::fem


#endif