#ifndef CIE_FEM_ABS_ELEMENT_HPP
#define CIE_FEM_ABS_ELEMENT_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "FEM/packages/discretization/inc/basis_wrappers.hpp"
#include "FEM/packages/discretization/inc/dof_map.hpp"
#include "FEM/packages/numeric/inc/basisfunctions.hpp"
#include "FEM/packages/utilities/inc/kernel.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::fem {



// ---------------------------------------------------------
// ABSTRACT BASE ELEMENT
// ---------------------------------------------------------

#define CIE_DOF_CONTAINER_TYPE std::vector<Size>


template <class BasisType>
class AbsElement : public DoFMap<CIE_DOF_CONTAINER_TYPE>
{
    // MEMBER TYPEDEFS -------------------------------------
public:
    typedef BasisType                                      basis_type;
    static const Size                                       dimension = BasisType::dimension;
    typedef typename BasisType::kernel_type                kernel_type;
    typedef typename kernel_type::number_type               NT;
    typedef std::array<NT,dimension>                        point_type;

    typedef std::vector<NT>                                 coefficient_container;
    typedef std::vector<NT>                                 value_container;
    typedef std::vector<point_type>                         point_container;
    typedef std::array<value_container,dimension>           basis_value_container;

    // MEMBER CLASSES --------------------------------------
public:
    // TODO: introduce a type-safe version of LocalCoordinates
    using LocalCoordinates = std::array<NT,dimension>;

    // CONSTRUCTORS ----------------------------------------
public:
    AbsElement( const typename AbsElement::dof_container& dofs );

    // BASIS -----------------------------------------------
public:
    /**
     * Construct basis function values from basis values
     *  - default implementation computes the tensor product
    */
    virtual void basis( const basis_value_container& basisValues,
                        value_container& outputContainer ) const;

    /**
     * Compute basis function derivatives from basis and basis derivative values
     * - default implementation computes the derivatives of the tensor product
    */
    virtual void basisDerivatives(  const basis_value_container& basisValues,
                                    const basis_value_container& basisDerivativeValues,
                                    point_container& outputContainer ) const;

    // COMPUTE FIELD VALUES --------------------------------
public:
    template <class CoefficientContainer, concepts::STLContainer BasisContainer>
    NT operator()(  const CoefficientContainer& coefficients,
                    const BasisContainer& basisValues ) const
    requires concepts::ClassContainer<CoefficientContainer,NT>
                && concepts::ClassContainer<typename BasisContainer::value_type,NT>;

    template <class CoefficientContainer>
    NT operator()(  const CoefficientContainer& coefficients,
                    const LocalCoordinates& point ) const
    requires concepts::ClassContainer<CoefficientContainer,NT>;

    template <  class CoefficientContainer,
                class PointIterator,
                class OutputIterator >
    void operator()(    const CoefficientContainer& coefficients,
                        PointIterator pointBegin,
                        PointIterator pointEnd,
                        OutputIterator outputIt )
    requires concepts::ClassContainer<CoefficientContainer,NT>
                && concepts::ClassIterator<PointIterator,LocalCoordinates>
                && concepts::ClassIterator<OutputIterator,NT>;

    // COMPUTE FIELD DERIVATIVES -------------------------------
protected:
    virtual void _derivative(   const coefficient_container& coefficients,
                                const basis_value_container& basisValues,
                                const basis_value_container& basisDerivativeValues,
                                point_type& gradient );

public:
    // Duplicate of AbsElement::_derivative to avoid unintentional shadowing when overriding it
    // in derived classes
    void derivative(    const coefficient_container& coefficients,
                        const basis_value_container& basisValues,
                        const basis_value_container& derivativeValues,
                        point_type& gradient );

    point_type derivative(  const coefficient_container& coefficients,
                            const LocalCoordinates& point );

    template <  class PointIterator,
                class OutputIterator >
    void derivative(    const coefficient_container& coefficients,
                        PointIterator pointBegin,
                        PointIterator pointEnd,
                        OutputIterator outputIt )
    requires concepts::ClassIterator<PointIterator,LocalCoordinates>
                && concepts::ClassIterator<OutputIterator,point_type>;


    // COORDINATE TRANSFORMATION -------------------------------
public:
    void localCoordinates(  const point_type& point,
                            LocalCoordinates& localPoint ) const;
    
    template <  class PointIterator,
                class LocalIterator >
    void localCoordinates(  PointIterator pointBegin,
                            PointIterator pointEnd,
                            LocalIterator localBegin ) const
    requires concepts::ClassIterator<PointIterator,point_type>
                && concepts::ClassIterator<LocalIterator,LocalCoordinates>;

    LocalCoordinates localCoordinates( const point_type& point ) const;

protected:
    virtual void toLocalCoordinates(    const point_type& point,
                                        LocalCoordinates& localPoint ) const = 0;

    // OTHER MEMBER FUNCTIONS ----------------------------------
public:
    const basis_type& basis() const                     { return _basisSet; }

    // MEMBER VARIABLES ----------------------------------------
protected:
    static basis_type   _basisSet;
}; // class AbsElement


// Initialize static members
template <class BasisType>
typename AbsElement<BasisType>::basis_type 
AbsElement<BasisType>::_basisSet = typename AbsElement<BasisType>::basis_type();





// ---------------------------------------------------------
// 1D BASE SPECIALIZATION
// ---------------------------------------------------------

template <class BasisType>
class AbsElement1D : public AbsElement<BasisType>
{
public:
    template <class ...Args>
    AbsElement1D(   const std::pair<typename AbsElement1D<BasisType>::NT, typename AbsElement1D<BasisType>::NT>& domain,
                    Args&&... );

protected:
    virtual void toLocalCoordinates(    const typename AbsElement1D<BasisType>::point_type& point,
                                        typename AbsElement1D<BasisType>::LocalCoordinates& localPoint ) const override;

    // Coordinate transformation specialized for 1D cases
    typename AbsElement1D<BasisType>::NT toLocalCoordinates( typename AbsElement1D<BasisType>::NT coordinate ) const;

    virtual void _derivative(   const typename AbsElement1D<BasisType>::coefficient_container& coefficients,
                                const typename AbsElement1D<BasisType>::basis_value_container& basisValues,
                                const typename AbsElement1D<BasisType>::basis_value_container& basisDerivativeValues,
                                typename AbsElement1D<BasisType>::point_type& gradient ) override;

protected:
    std::pair<typename AbsElement1D<BasisType>::NT, typename AbsElement1D<BasisType>::NT> _domain;
    typename AbsElement1D<BasisType>::NT                                                  _jacobian;
    typename AbsElement1D<BasisType>::NT                                                  _invJacobian;
}; // class AbsElement1D


} // namespace cie::fem

#include "FEM/packages/discretization/impl/abs_element_impl.hpp"

#endif