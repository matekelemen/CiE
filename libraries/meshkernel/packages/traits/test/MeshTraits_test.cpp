// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/traits/inc/MeshTraits.hpp"


namespace cie::mesh {


template <detail::HasMeshTraits T>
bool hasMeshTraits( const T& r_object )
{
    return true;
}


template <class T>
requires (!detail::HasMeshTraits<T>)
bool hasMeshTraits( const T& r_object )
{
    return false;
}


CIE_TEST_CASE( "Traits", "[traits]" )
{
    CIE_TEST_CASE_INIT( "Traits" )

    double scalar = 0.0;
    csg::CSGTraits<1,double> csgTraits;
    MeshTraits<1,double> meshTraits;

    CIE_TEST_CHECK( !hasMeshTraits(scalar) );
    CIE_TEST_CHECK( hasMeshTraits(csgTraits) );
    CIE_TEST_CHECK( hasMeshTraits(meshTraits) );
}


} // namespace cie::mesh