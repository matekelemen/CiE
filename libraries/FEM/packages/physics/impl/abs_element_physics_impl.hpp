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
    utils::setContainerSize( integrationPoints, _integratorPtr->integrationPoints().size() );
    std::copy(  _integratorPtr->integrationPoints().begin(),
                _integratorPtr->integrationPoints().end(),
                integrationPoints.begin() );
    
    // Cache basis values and its derivatives
    typename ElementType::ansatz_value_container                    ansatzValues;
    typename ElementType::ansatz_value_container                    ansatzDerivativeValues;
    typename AbsElementPhysics::basis_set_container                 basisSet;
    typename AbsElementPhysics::basis_derivative_set_container      basisDerivativeSet;
    auto ansatzDerivatives = this->_ansatzSet.derivatives();

    for (const auto& point : integrationPoints)
    {
        // Compute ansatz values and their derivatives
        auto ansatzIt               = ansatzValues.begin();
        auto ansatzDerivativeIt     = ansatzDerivativeValues.begin();

        for (Size dim=0; dim<ElementType::dimension; ++dim,++ansatzIt,++ansatzDerivativeIt)
        {
            ansatzIt->clear();
            ansatzDerivativeIt->clear();
            const auto& coordinate = point[dim];
            for (const auto& function : this->_ansatzSet.functions(dim))
                ansatzIt->push_back( function(coordinate) );
            for (const auto& function : ansatzDerivatives->functions(dim))
                ansatzDerivativeIt->push_back( function(coordinate) );
        }

        // Compute basis and basis derivative values
        basisSet.emplace_back();
        basisDerivativeSet.emplace_back();

        this->basis(    ansatzValues,
                        basisSet.back() );
        this->basisDerivatives( ansatzValues,
                                ansatzDerivativeValues,
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