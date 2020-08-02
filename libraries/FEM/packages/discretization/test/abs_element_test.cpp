// --- External Includes ---
#include "catch.hpp"

// --- Linalg Includes ---
#include <linalg/overloads.hpp>

// --- Internal Includes ---
#include "../inc/abs_element.hpp"
#include "../../numeric/inc/basisfunctions.hpp"

// --- STL Includes ---
#include <iostream>


namespace cie::fem {


namespace detail {
template <class BasisType>
struct TestElement : public AbsElement<BasisType>
{
protected:
    void toLocalCoordinates(    const typename TestElement::point_type& point,
                                TestElement::LocalCoordinates& localPoint ) const override
    {
        std::copy(  point.begin(),
                    point.end(),
                    localPoint.begin() );
    }

    void _derivative(   const typename TestElement::coefficient_container& coefficients,
                        const std::array<typename TestElement::basis_value_container,TestElement::dimension>& basisValues,
                        const std::array<typename TestElement::basis_value_container,TestElement::dimension>& derivativeValues,
                        typename TestElement::point_type& gradient ) override
    {
        std::fill(  gradient.begin(),
                    gradient.end(),
                    0.0 );
        auto basisDerivatives   = detail::makeTensorProductDerivatives(basisValues, derivativeValues);
        for (const auto& coefficient : coefficients)
        {
            auto basisDerivativeValues  = *basisDerivatives;
            for (Size dim=0; dim<this->dimension; ++dim)
                gradient[dim] += basisDerivativeValues[dim] * coefficient;

            ++basisDerivatives;
        }
    }
};
}



TEST_CASE( "AbsElement", "[discretization]" )
{
    // General init
    const Size                                      Dimension = 2;
    typedef Double                                  NT;
    typedef LinearBasisFunctionSet<Dimension,NT>    Basis;
    typedef detail::TestElement<Basis>              Element;

    // Test constructor
    REQUIRE_NOTHROW( Element() );
    Element element;

    // Compute reference points
    Size resolution = 5;
    std::vector<typename Element::point_type> points;
    for (Size i=0; i<resolution; ++i)
        for (Size j=0; j<resolution; ++j)
            points.push_back(typename Element::point_type({
                NT(-1.0) + NT(i)*2.0/NT(resolution-1),
                NT(-1.0) + NT(j)*2.0/NT(resolution-1)
            }));

    // Transform reference points to local coordinates
    std::vector<Element::LocalCoordinates> localPoints;
    utils::setContainerSize(localPoints,points.size());
    CHECK_NOTHROW( element.localCoordinates(    points.begin(),
                                                points.end(),
                                                localPoints.begin()) );

    // Reference coefficients
    std::vector<NT> coefficients    = { 1.0, 1.0, 1.0, 1.0 };

    // Test field values
    for (const auto& point : localPoints)
    {
        auto xi     = point[0];
        auto eta    = point[1];
        auto reference =    coefficients[0] * (1.0-xi)*(1.0-eta)/4.0
                            + coefficients[1] * (1.0+xi)*(1.0-eta)/4.0
                            + coefficients[2] * (1.0-xi)*(1.0+eta)/4.0
                            + coefficients[3] * (1.0+xi)*(1.0+eta)/4.0;
        REQUIRE_NOTHROW( element(coefficients, point) );
        CHECK( element(coefficients,point) == Approx(reference) );
    }

    // Test field derivatives
    for (const auto& point : localPoints)
    {
        auto xi     = point[0];
        auto eta    = point[1];
        std::array<NT,Dimension> reference = {
            coefficients[0] * (eta-1.0)/4.0
            + coefficients[1] * (1.0-eta)/4.0
            + coefficients[2] * -(eta+1.0)/4.0
            + coefficients[3] * (eta+1.0)/4.0
            ,
            coefficients[0] * (xi-1.0)/4.0
            + coefficients[1] * -(1.0+xi)/4.0
            + coefficients[2] * (1.0-xi)/4.0
            + coefficients[3] * (xi+1.0)/4.0,
        };
        REQUIRE_NOTHROW( element.derivative(coefficients,point) );
        auto test = element.derivative(coefficients,point);
        for (Size i=0; i<Dimension; ++i)
            CHECK( test[i] == Approx(reference[i]) );
        //    std::cout << test[i] << "\t";
        //std::cout << "\n";
    }
}


} // namespace cie::fem