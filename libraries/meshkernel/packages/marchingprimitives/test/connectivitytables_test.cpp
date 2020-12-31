// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/connectivitytables.hpp"


namespace cie::mesh {


CIE_TEST_CASE( "marchingCubesConnectivityMap", "[marchingprimitives]" )
{
    CIE_TEST_CASE_INIT( "marchingCubesConnectivityMap" )

    auto table = detail::marchingCubesConnectivityMap;
}


} // namespace cie::mesh