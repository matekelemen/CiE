#ifndef CIE_FEM_ELEMENT_HPP
#define CIE_FEM_ELEMENT_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- Internal Includes ---
#include "./element.hpp"
#include "../../utilities/inc/kernel.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::utils {


template <  Size Dimension,
            concepts::NumericType NT >
class AbsElement
{
public:
    static const Size                           dimension = Dimension;
    typedef Kernel<NT>                          kernel_type;
    typedef AbsBasisFunctionSet<Dimension,NT>   basis_interface;

public:
    AbsElement();

    template <class CoefficientContainer, class PointType>
    NT operator()(  const CoefficientContainer& coefficients
                    const PointType& point ) const;
    requires concepts::ClassContainer<PointType,NT>;

    template <  class CoefficientContainer,
                class OutputIterator,
                template <class ...> class PointContainer, class PointType, class ...Args >
    void operator()(    const CoefficientContainer& coefficients
                        const PointContainer<PointType,Args...>& points,
                        OutputIterator outputIt )
    requires concepts::ClassContainer<CoefficientContainer,NT>
                && concepts::ClassContainer<PointType,NT>
                && concepts::ClassIterator<OutputIterator,NT>;

    std::shared_ptr<basis_interface>& basis()               { return _basis; };
    const std::shared_ptr<basis_interface> basis() const    { return _basis };

protected:
    std::shared_ptr<basis_interface>    _basis;
};


}

#include "../inc/element.hpp"

#endif