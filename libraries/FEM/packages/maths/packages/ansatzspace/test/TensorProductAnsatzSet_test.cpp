// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/ansatzspace/inc/TensorProductAnsatzSet.hpp"
#include "FEM/packages/maths/packages/polynomial/inc/UnivariatePolynomial.hpp"


namespace cie::fem::maths {


CIE_TEST_CASE( "TensorProductAnsatzSpace", "[maths]" )
{
    CIE_TEST_CASE_INIT( "TensorProductAnsatzSpace" )
    
    using BasisType = UnivariatePolynomial<double>;
    using Coefficients = BasisType::coefficient_container;
    using BasisPtr = std::shared_ptr<BasisType>;
    using AnsatzType = TensorProductAnsatzSet<2,double>;

    std::vector<BasisPtr> basisFunctions {
        BasisPtr( new BasisType(Coefficients {0.5, -0.5}) ),
        BasisPtr( new BasisType(Coefficients {0.5, 0.5}) ),
        BasisPtr( new BasisType(Coefficients {0.0, 0.0, 1.0}) )
    };

    CIE_TEST_REQUIRE_NOTHROW( AnsatzType(basisFunctions) );
    AnsatzType ansatzSet( basisFunctions );
    CIE_TEST_REQUIRE( ansatzSet.size() == 9 );

    using Point = Kernel<2,double>::point_type;
    double x = 11.0;
    double y = 9.0;
    Point p0 { -x, -y };
    Point p1 { -x, y };
    Point p2 { x, -y };
    Point p3 { x, y };

    CIE_TEST_CHECK( ansatzSet[0](p0) == Approx(30.0) );
    CIE_TEST_CHECK( ansatzSet[0](p1) == Approx(-24.0) );
    CIE_TEST_CHECK( ansatzSet[0](p2) == Approx(-25.0) );
    CIE_TEST_CHECK( ansatzSet[0](p3) == Approx(20.0) );

    CIE_TEST_CHECK( ansatzSet[3](p0) == Approx(-24.0) );
    CIE_TEST_CHECK( ansatzSet[3](p1) == Approx(30.0) );
    CIE_TEST_CHECK( ansatzSet[3](p2) == Approx(20.0) );
    CIE_TEST_CHECK( ansatzSet[3](p3) == Approx(-25.0) );

    CIE_TEST_CHECK( ansatzSet[6](p0) == Approx(486.0) );
    CIE_TEST_CHECK( ansatzSet[6](p1) == Approx(486.0) );
    CIE_TEST_CHECK( ansatzSet[6](p2) == Approx(-405.0) );
    CIE_TEST_CHECK( ansatzSet[6](p3) == Approx(-405.0) );

    CIE_TEST_CHECK( ansatzSet[1](p0) == Approx(-25.0) );
    CIE_TEST_CHECK( ansatzSet[1](p1) == Approx(20.0) );
    CIE_TEST_CHECK( ansatzSet[1](p2) == Approx(30.0) );
    CIE_TEST_CHECK( ansatzSet[1](p3) == Approx(-24.0) );

    CIE_TEST_CHECK( ansatzSet[4](p0) == Approx(20.0) );
    CIE_TEST_CHECK( ansatzSet[4](p1) == Approx(-25.0) );
    CIE_TEST_CHECK( ansatzSet[4](p2) == Approx(-24.0) );
    CIE_TEST_CHECK( ansatzSet[4](p3) == Approx(30.0) );

    CIE_TEST_CHECK( ansatzSet[7](p0) == Approx(-405.0) );
    CIE_TEST_CHECK( ansatzSet[7](p1) == Approx(-405.0) );
    CIE_TEST_CHECK( ansatzSet[7](p2) == Approx(486.0) );
    CIE_TEST_CHECK( ansatzSet[7](p3) == Approx(486.0) );

    CIE_TEST_CHECK( ansatzSet[2](p0) == Approx(605.0) );
    CIE_TEST_CHECK( ansatzSet[2](p1) == Approx(-484.0) );
    CIE_TEST_CHECK( ansatzSet[2](p2) == Approx(605.0) );
    CIE_TEST_CHECK( ansatzSet[2](p3) == Approx(-484.0) );

    CIE_TEST_CHECK( ansatzSet[5](p0) == Approx(-484.0) );
    CIE_TEST_CHECK( ansatzSet[5](p1) == Approx(605.0) );
    CIE_TEST_CHECK( ansatzSet[5](p2) == Approx(-484.0) );
    CIE_TEST_CHECK( ansatzSet[5](p3) == Approx(605.0) );

    CIE_TEST_CHECK( ansatzSet[8](p0) == Approx(9801.0) );
    CIE_TEST_CHECK( ansatzSet[8](p1) == Approx(9801.0) );
    CIE_TEST_CHECK( ansatzSet[8](p2) == Approx(9801.0) );
    CIE_TEST_CHECK( ansatzSet[8](p3) == Approx(9801.0) );
}


} // namespace cie::fem::maths