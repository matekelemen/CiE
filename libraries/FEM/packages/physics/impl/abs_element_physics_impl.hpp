#ifndef CIE_FEM_ABS_PHYSICS_POLICY_IMPL_HPP
#define CIE_FEM_ABS_PHYSICS_POLICY_IMPL_HPP


namespace cie::fem {


template <  concepts::STLContainer NestedContainer,
            concepts::STLContainer StoredType >
requires concepts::STLContainer<typename NestedContainer::value_type>
inline Size
NestedCache<NestedContainer,StoredType>::hash( const NestedContainer& container ) const
{
    Size seed = container.size();
    for ( auto it=container.begin(); it!=container.end(); it++ )
        seed ^= Size(&(*it)) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    return seed;
}


template <class ElementType>
template <class ...Args>
AbsElementPhysics<ElementType>::AbsElementPhysics( Args&&... elementArgs ) :
    ElementType( std::forward<Args>(elementArgs)... )
{
    // Set default integrator as a second-order gauss-legendre quadrature
    setIntegrator<GaussLegendreQuadrature<ElementType::dimension,typename AbsElementPhysics::NT>>(2);
}


template <class ElementType>
template <class IntegratorType, class ...Args>
typename AbsElementPhysics<ElementType>::integrator_const_reference
AbsElementPhysics<ElementType>::setIntegrator( Args&&... integratorArgs )
{
    _integratorPtr = std::make_shared<IntegratorType>( std::forward<Args>(integratorArgs)... );
    this->recache();
    return *_integratorPtr;
}


template <class ElementType>
void
AbsElementPhysics<ElementType>::recache()
{
    // Get integration points
    typename ElementType::point_container integrationPoints;
    cie::utils::resize( integrationPoints, _integratorPtr->integrationPoints().size() );
    std::copy(  _integratorPtr->integrationPoints().begin(),
                _integratorPtr->integrationPoints().end(),
                integrationPoints.begin() );
    
    // Cache basis values and its derivatives
    typename ElementType::basis_value_container                    basisValues;
    typename ElementType::basis_value_container                    basisDerivativeValues;
    typename AbsElementPhysics::basis_set_container                 basisSet;
    typename AbsElementPhysics::basis_derivative_set_container      basisDerivativeSet;
    auto basisDerivatives = this->_basisSet.derivatives();

    for (const auto& point : integrationPoints)
    {
        // Compute basis values and their derivatives
        auto basisIt               = basisValues.begin();
        auto basisDerivativeIt     = basisDerivativeValues.begin();

        for (Size dim=0; dim<ElementType::dimension; ++dim,++basisIt,++basisDerivativeIt)
        {
            basisIt->clear();
            basisDerivativeIt->clear();
            const auto& coordinate = point[dim];
            for (const auto& function : this->_basisSet.functions(dim))
                basisIt->push_back( function(coordinate) );
            for (const auto& function : basisDerivatives->functions(dim))
                basisDerivativeIt->push_back( function(coordinate) );
        }

        // Compute basis and basis derivative values
        basisSet.emplace_back();
        basisDerivativeSet.emplace_back();

        this->basis(    basisValues,
                        basisSet.back() );
        this->basisDerivatives( basisValues,
                                basisDerivativeValues,
                                basisDerivativeSet.back() );
    }

    // Store computed values
    this->_cacheID = this->_basisCache.insert(  integrationPoints,
                                                basisSet,
                                                true )->first;
    this->_basisDerivativeCache.insert( integrationPoints,
                                        basisDerivativeSet,
                                        true );
} // void recache()


} // namespace cie::fem


#endif