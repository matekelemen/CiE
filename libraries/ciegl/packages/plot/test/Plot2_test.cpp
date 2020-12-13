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


TEST_CASE( "Plot2", "[plot]" )
{
    CIE_TEST_CASE_INIT( "Plot2" )

    std::deque<double> X;
    std::vector<double> Y;

    Size n    = 100;
    double dx = 2.0 * M_PI / double(n);

    for ( double x=0.0; x<=2.0*M_PI; x+=dx )
    {
        X.push_back( x );
        Y.push_back( std::cos(x) );
    }

    plot( X, Y );
}


} // namespace cie::gl