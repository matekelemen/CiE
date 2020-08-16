#ifndef CIE_FEM_ABS_ELEMENT_HPP
#define CIE_FEM_ABS_ELEMENT_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- Internal Includes ---
#include "../../numeric/inc/basisfunctions.hpp"
#include "./basis_wrappers.hpp"
#include "./dof_map.hpp"
#include "../../utilities/inc/kernel.hpp"
#include "../../numeric/inc/integration.hpp"

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
    typedef BasisType                                       basis_type;
    static const Size                                       dimension = BasisType::dimension;
    typedef typename BasisType::kernel_type                 kernel_type;
    typedef typename kernel_type::number_type               NT;
    typedef std::array<NT,dimension>                        point_type;

    typedef std::vector<NT>                                 coefficient_container;
    typedef std::vector<NT>                                 basis_value_container;
    typedef std::unique_ptr<AbsQuadrature<dimension,NT>>    integrator_ptr;         

    // MEMBER CLASSES --------------------------------------
public:
    struct LocalCoordinates : public std::array<NT,dimension> {};

    // CONSTRUCTORS ----------------------------------------
public:
    AbsElement( const typename AbsElement::dof_container& dofs );

    // COMPUTE FIELD VALUES --------------------------------
public:
    template <class CoefficientContainer, class BasisContainer>
    NT operator()(  const CoefficientContainer& coefficients,
                    const std::array<BasisContainer,dimension>& basisValues ) const
    requires concepts::ClassContainer<CoefficientContainer,NT>
                && concepts::ClassContainer<BasisContainer,NT>;

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
                                const std::array<basis_value_container,dimension>& basisValues,
                                const std::array<basis_value_container,dimension>& derivativeValues,
                                std::array<NT,dimension>& gradient );

public:
    // Duplicate of AbsElement::_derivative to avoid unintentional shadowing when overriding it
    // in derived classes
    void derivative(    const coefficient_container& coefficients,
                        const std::array<basis_value_container,dimension>& basisValues,
                        const std::array<basis_value_container,dimension>& derivativeValues,
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
    const basis_type& basis() const                     { return _basis; }

    // MEMBER VARIABLES ----------------------------------------
protected:
    integrator_ptr      _integratorPtr;
    static basis_type   _basis;
}; // class AbsElement


// Initialize static members
template <class BasisType>
typename AbsElement<BasisType>::basis_type 
AbsElement<BasisType>::_basis = typename AbsElement<BasisType>::basis_type();





// ---------------------------------------------------------
// 1D BASE SPECIALIZATION
// ---------------------------------------------------------

template <class BasisType>
class AbsElement1D : public AbsElement<BasisType>
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
                                const std::array<typename AbsElement1D::basis_value_container,AbsElement1D::dimension>& basisValues,
                                const std::array<typename AbsElement1D::basis_value_container,AbsElement1D::dimension>& derivativeValues,
                                std::array<typename AbsElement1D::NT,AbsElement1D::dimension>& gradient ) override;

protected:
    std::pair<typename AbsElement1D::NT, typename AbsElement1D::NT> _domain;
    typename AbsElement1D::NT                                       _jacobian;
    typename AbsElement1D::NT                                       _invJacobian;
}; // class AbsElement1D


} // namespace cie::fem

#include "../impl/abs_element_impl.hpp"

#endif