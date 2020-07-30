#ifndef CIE_FEM_ELEMENT_HPP
#define CIE_FEM_ELEMENT_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- Internal Includes ---
#include "../../numeric/inc/basisfunctions.hpp"
#include "../../utilities/inc/kernel.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::fem {



// ---------------------------------------------------------
// ABSTRACT BASE ELEMENT
// ---------------------------------------------------------

template <class BasisType>
class AbsElement
{
public:
    typedef BasisType                           basis_type;
    static const Size                           dimension = BasisType::dimension;
    typedef typename BasisType::kernel_type     kernel_type;
    typedef typename kernel_type::number_type   NT;

public:     // <--- member classes
    struct LocalCoordinates : public std::array<NT,dimension> {};

public:
    template <class CoefficientContainer>
    NT operator()(  const CoefficientContainer& coefficients,
                    const LocalCoordinates& point ) const;

    template <  class CoefficientContainer,
                class OutputIterator >
    void operator()(    const CoefficientContainer& coefficients,
                        const LocalCoordinates& points,
                        OutputIterator outputIt )
    requires concepts::ClassContainer<CoefficientContainer,NT>
                && concepts::ClassIterator<OutputIterator,NT>;

    virtual void toLocalCoordinates(    const std::array<NT,dimension>& point,
                                        LocalCoordinates& localCoordinates ) const = 0;

    basis_type& basis()                                 { return _basis; }
    const basis_type& basis() const                     { return _basis; }

protected:
    static basis_type       _basis = basis_type();
};


// ---------------------------------------------------------
// 1D BASE SPECIALIZATION
// ---------------------------------------------------------


}

#include "../inc/element.hpp"

#endif