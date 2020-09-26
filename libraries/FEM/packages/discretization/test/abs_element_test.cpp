// --- External Includes ---
#include "catch.hpp"

// --- Linalg Includes ---
#include "linalg/packages/overloads/inc/vectoroperators.hpp"

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- Internal Includes ---
#include "FEM/packages/discretization/inc/abs_element.hpp"
#include "FEM/packages/numeric/inc/basisfunctions.hpp"

// --- STL Includes ---
#include <iostream>


namespace cie::fem {


namespace detail {
template <class BasisType>
struct TestElement : public AbsElement<BasisType>
{
public:
    TestElement() :
        AbsElement<BasisType>( typename AbsElement<BasisType>::dof_container({0,1,2,3}) ) 
    {}

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
    utils::resize(localPoints,points.size());
    CHECK_NOTHROW( element.localCoordinates(    points.begin(),
                                                points.end(),
                                                localPoints.begin()) );

    // Reference coefficients
    const std::vector<NT> coefficients    = { 1.0, 2.0, 3.0, 4.0 };

    // Test field values
    for (const auto& point : localPoints)
    {
        auto xi     = point[0];
        auto eta    = point[1];
        auto reference =    coefficients[0] * (1.0+xi)*(1.0+eta)/4.0
                            + coefficients[1] * (1.0+xi)*(1.0-eta)/4.0
                            + coefficients[2] * (1.0-xi)*(1.0+eta)/4.0
                            + coefficients[3] * (1.0-xi)*(1.0-eta)/4.0;
        REQUIRE_NOTHROW( element(coefficients, point) );
        CHECK( element(coefficients,point) == Approx(reference) );
    }

    // Test field derivatives
    for (const auto& point : localPoints)
    {
        auto xi     = point[0];
        auto eta    = point[1];
        std::array<NT,Dimension> reference = {
            coefficients[0] * (1.0+eta)/4.0
            + coefficients[1] * (1.0-eta)/4.0
            + coefficients[2] * -(1.0+eta)/4.0
            + coefficients[3] * -(1.0-eta)/4.0
            ,
            coefficients[0] * (1.0+xi)/4.0
            + coefficients[1] * -(1.0+xi)/4.0
            + coefficients[2] * (1.0-xi)/4.0
            + coefficients[3] * -(1.0-xi)/4.0,
        };
        REQUIRE_NOTHROW( element.derivative(coefficients,point) );
        auto test = element.derivative(coefficients,point);
        for (Size i=0; i<Dimension; ++i)
            CHECK( test[i] == Approx(reference[i]) );
    }
} // TEST_CASE AbsElement



TEST_CASE( "AbsElement1D", "[discretization]" )
{
    // General init
    const Size                                      Dimension = 1;
    typedef Double                                  NT;
    typedef LinearBasisFunctionSet<Dimension,NT>    Basis;
    typedef AbsElement1D<Basis>                     Element;

    const Size                                      resolution = 5;

    // Test constructor
    REQUIRE_NOTHROW(
    Element(    std::pair<NT,NT>(0.0,1.0),
                typename Element::dof_container({0,1}) )
    );

    Element element(    std::pair<NT,NT>(0.0,1.0),
                        typename Element::dof_container({0,1}) );

    // Generate reference points
    std::vector<typename Element::point_type> points;
    for (Size i=0; i<resolution; ++i)
        points.push_back( typename Element::point_type({ NT(i)/NT(resolution-1) }) );

    // Basis function coefficients
    typename Element::coefficient_container coefficients( {2.0,3.0} );

    // Convert points to local coordinates
    std::vector<typename Element::LocalCoordinates> localPoints;
    utils::resize( localPoints, points.size() );
    REQUIRE_NOTHROW(
        element.localCoordinates(   points.begin(),
                                    points.end(),
                                    localPoints.begin() )
    );

    // Check field values
    {
        for (const auto& point : localPoints)
        {
            CHECK(
                element( coefficients, point ) 
                == Approx(
                coefficients[0] * (point[0] + 1.0)/2.0
                + coefficients[1] * (-point[0] + 1.0)/2.0 )
            );
        }
    }

    // Check field derivatives
    {
        for (const auto& point : localPoints)
        {
            CHECK(
                element.derivative( coefficients, point )[0]
                == Approx( coefficients[0] - coefficients[1] )
            );
        }
    }
}


} // namespace cie::fem