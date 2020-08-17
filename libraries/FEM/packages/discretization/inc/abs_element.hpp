#ifndef CIE_FEM_ABS_ELEMENT_HPP
#define CIE_FEM_ABS_ELEMENT_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- Internal Includes ---
#include "../../numeric/inc/ansatzfunctions.hpp"
#include "./basis_wrappers.hpp"
#include "./dof_map.hpp"
#include "../../utilities/inc/kernel.hpp"
#include "../../numeric/inc/integration.hpp"


namespace cie::fem {



// ---------------------------------------------------------
// ABSTRACT BASE ELEMENT
// ---------------------------------------------------------

#define CIE_DOF_CONTAINER_TYPE std::vector<Size>


template <class AnsatzType>
class AbsElement : public DoFMap<CIE_DOF_CONTAINER_TYPE>
{
    // MEMBER TYPEDEFS -------------------------------------
public:
    typedef AnsatzType                                      ansatz_type;
    static const Size                                       dimension = AnsatzType::dimension;
    typedef typename AnsatzType::kernel_type                kernel_type;
    typedef typename kernel_type::number_type               NT;
    typedef std::array<NT,dimension>                        point_type;

    typedef std::vector<NT>                                 coefficient_container;
    typedef std::vector<NT>                                 value_container;
    typedef std::vector<point_type>                         point_container;
    typedef std::array<value_container,dimension>           ansatz_value_container;

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
     * Construct basis function values from ansatz values
     *  - default implementation computes the tensor product
    */
    virtual void basis( const ansatz_value_container& ansatzValues,
                        value_container& outputContainer ) const;

    /**
     * Compute basis function derivatives from ansatz and ansatz derivative values
     * - default implementation computes the derivatives of the tensor product
    */
    virtual void basisDerivatives(  const ansatz_value_container& ansatzValues,
                                    const ansatz_value_container& ansatzDerivativeValues,
                                    point_container& outputContainer ) const;

    // COMPUTE FIELD VALUES --------------------------------
public:
    template <class CoefficientContainer, concepts::STLContainer AnsatzContainer>
    NT operator()(  const CoefficientContainer& coefficients,
                    const AnsatzContainer& ansatzValues ) const
    requires concepts::ClassContainer<CoefficientContainer,NT>
                && concepts::ClassContainer<typename AnsatzContainer::value_type,NT>;

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
                                const ansatz_value_container& ansatzValues,
                                const ansatz_value_container& ansatzDerivativeValues,
                                point_type& gradient );

public:
    // Duplicate of AbsElement::_derivative to avoid unintentional shadowing when overriding it
    // in derived classes
    void derivative(    const coefficient_container& coefficients,
                        const ansatz_value_container& ansatzValues,
                        const ansatz_value_container& derivativeValues,
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
    const ansatz_type& basis() const                     { return _ansatzSet; }

    // MEMBER VARIABLES ----------------------------------------
protected:
    static ansatz_type   _ansatzSet;
}; // class AbsElement


// Initialize static members
template <class AnsatzType>
typename AbsElement<AnsatzType>::ansatz_type 
AbsElement<AnsatzType>::_ansatzSet = typename AbsElement<AnsatzType>::ansatz_type();





// ---------------------------------------------------------
// 1D BASE SPECIALIZATION
// ---------------------------------------------------------

template <class AnsatzType>
class AbsElement1D : public AbsElement<AnsatzType>
{
public:
    template <class ...Args>
    AbsElement1D(   const std::pair<typename AbsElement1D::NT, typename AbsElement1D::NT>& domain,
                    Args&&... );

protected:
    virtual void toLocalCoordinates(    const typename AbsElement1D::point_type& point,
                                        AbsElement1D::LocalCoordinates& localPoint ) const override;

    // Coordinate transformation specialized for 1D cases
    typename AbsElement1D::NT toLocalCoordinates( typename AbsElement1D::NT coordinate ) const;

    virtual void _derivative(   const typename AbsElement1D::coefficient_container& coefficients,
                                const typename AbsElement1D::ansatz_value_container& ansatzValues,
                                const typename AbsElement1D::ansatz_value_container& ansatzDerivativeValues,
                                typename AbsElement1D::point_type& gradient ) override;

protected:
    std::pair<typename AbsElement1D::NT, typename AbsElement1D::NT> _domain;
    typename AbsElement1D::NT                                       _jacobian;
    typename AbsElement1D::NT                                       _invJacobian;
}; // class AbsElement1D


} // namespace cie::fem

#include "../impl/abs_element_impl.hpp"

#endif