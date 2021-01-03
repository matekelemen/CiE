#define _USE_MATH_DEFINES

// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/plot/inc/Plot2.hpp"

// --- STL Includes ---
#include <deque>
#include <vector>
#include <math.h>
#include <cmath>


namespace cie::gl {


CIE_TEST_CASE( "Plot2", "[plot]" )
{
    CIE_TEST_CASE_INIT( "Plot2" )

    using XContainer = std::deque<Double>;
    using YContainer = std::vector<Double>;
    XContainer X;
    YContainer Y;

    Size n    = 100;
    double dx = 2.0 * M_PI / double(n);

    for ( double x=0.0; x<=2.0*M_PI; x+=dx )
    {
        X.push_back( x );
        Y.push_back( std::cos(x) );
    }

    plot2( X, Y );
}


} // namespace cie::gl