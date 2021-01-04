// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/concepts.hpp"
#include "meshkernel/packages/marchingprimitives/inc/MarchingCubes.hpp"


namespace cie::mesh {


template <concepts::MarchingPrimitives T>
bool isMarchingPrimitives()
{ return true; }


template <class T>
requires (!concepts::MarchingPrimitives<T>)
bool isMarchingPrimitives()
{ return false; }


CIE_TEST_CASE( "concepts", "[marchingprimitives]" )
{
    CIE_TEST_CASE_INIT( "concepts" )

    const Size Dimension     = 2;
    using CoordinateType     = Double;
    using TestTraits         = MeshTraits<Dimension,CoordinateType>;
    using TargetType         = csg::CSGObject<Dimension,Bool,CoordinateType>;
    using MarchingPrimitives = UnstructuredMarchingCubes<TargetType>;

    CIE_TEST_CHECK( isMarchingPrimitives<MarchingPrimitives>() );
    CIE_TEST_CHECK( !isMarchingPrimitives<TestTraits>() );
}


} // namespace cie::mesh