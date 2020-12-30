// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/structured/inc/cartesianmesh.hpp"


namespace cie::mesh {


CIE_TEST_CASE( "makeCartesianMesh", "[structured]" )
{
    CIE_TEST_CASE_INIT( "makeCartesianMesh - Cube" )

    const Size Dimension = 2;
    using CoordinateType = Double;
    using TestTraits     = MeshTraits<Dimension,CoordinateType>;

    {
        CIE_TEST_CASE_INIT( "Cube" )

        using PrimitiveType = csg::Cube<Dimension,CoordinateType>;

        TestTraits::resolution_specifier numberOfPrimitives { 2, 4 };
        CoordinateType edgeLength = 1.0;
        TestTraits::point_type origin { 0.0, 0.0 };

        CIE_TEST_REQUIRE_NOTHROW(
            makeCartesianMesh<PrimitiveType>( numberOfPrimitives,
                                              edgeLength,
                                              origin )
        );

        auto mesh = makeCartesianMesh<PrimitiveType>( numberOfPrimitives,
                                                      edgeLength,
                                                      origin );

        CIE_TEST_REQUIRE( mesh.size() == numberOfPrimitives[0] * numberOfPrimitives[1] );

        auto p_primitive = mesh[0];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->length() == Approx(edgeLength) );

        p_primitive = mesh[1];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->length() == Approx(edgeLength) );

        p_primitive = mesh[2];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->length() == Approx(edgeLength) );

        p_primitive = mesh[3];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->length() == Approx(edgeLength) );

        p_primitive = mesh[4];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(2.0) );
        CIE_TEST_CHECK( p_primitive->length() == Approx(edgeLength) );

        p_primitive = mesh[5];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(2.0) );
        CIE_TEST_CHECK( p_primitive->length() == Approx(edgeLength) );

        p_primitive = mesh[6];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(3.0) );
        CIE_TEST_CHECK( p_primitive->length() == Approx(edgeLength) );

        p_primitive = mesh[7];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(3.0) );
        CIE_TEST_CHECK( p_primitive->length() == Approx(edgeLength) );
    }

    {
        CIE_TEST_CASE_INIT( "Box" )

        using PrimitiveType = csg::Box<Dimension,CoordinateType>;

        TestTraits::domain_specifier domain {{ {0.0,2.0}, {0.0,4.0} }};
        TestTraits::resolution_specifier resolution { 2, 4 };
        
        CIE_TEST_REQUIRE_NOTHROW(
            makeCartesianMesh<PrimitiveType>( domain, resolution )
        );

        auto mesh = makeCartesianMesh<PrimitiveType>( domain, resolution );

        CIE_TEST_REQUIRE( mesh.size() == resolution[0] * resolution[1] );

        auto p_primitive = mesh[0];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[1] == Approx(1.0) );

        p_primitive = mesh[1];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[1] == Approx(1.0) );

        p_primitive = mesh[2];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[1] == Approx(1.0) );

        p_primitive = mesh[3];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[1] == Approx(1.0) );

        p_primitive = mesh[4];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(2.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[1] == Approx(1.0) );

        p_primitive = mesh[5];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(2.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[1] == Approx(1.0) );

        p_primitive = mesh[6];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(0.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(3.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[1] == Approx(1.0) );

        p_primitive = mesh[7];
        CIE_TEST_CHECK( p_primitive->base()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->base()[1] == Approx(3.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[0] == Approx(1.0) );
        CIE_TEST_CHECK( p_primitive->lengths()[1] == Approx(1.0) );
    }
}


} // namespace cie::mesh