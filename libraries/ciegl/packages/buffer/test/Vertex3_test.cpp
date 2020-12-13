// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/Vertex3.hpp"


namespace cie::gl {


TEST_CASE( "Vertex3", "[buffer]" )
{
    CIE_TEST_CASE_INIT( "Vertex3" )

    auto p_attributes = AttributeContainerPtr( new AttributeContainer );

    REQUIRE_NOTHROW( makeVertex<Vertex3>( p_attributes ) );

    Vertex3Ptr p_vertex;
    REQUIRE_NOTHROW( p_vertex = makeVertex<Vertex3>( p_attributes ) );

    CHECK_NOTHROW( p_vertex->x() = 1.0 );
    CHECK_NOTHROW( p_vertex->y() = 2.0 );
    CHECK_NOTHROW( p_vertex->z() = 3.0 );
    CHECK( p_vertex->x() == Approx(1.0) );
    CHECK( p_vertex->y() == Approx(2.0) );
    CHECK( p_vertex->z() == Approx(3.0) );

    REQUIRE_NOTHROW( p_vertex->position() );
    Size counter = 0;
    for ( const auto& component : p_vertex->position() )
        CHECK( component == Approx( ++counter ) );

    for ( auto& component : p_vertex->position() )
    {
        double tmp = (counter+1) / double(counter+2);
        component = tmp;
        counter += 2;
    }

    CHECK( p_vertex->x() == Approx(4.0 / 5.0) );
    CHECK( p_vertex->y() == Approx(6.0 / 7.0) );
    CHECK( p_vertex->z() == Approx(8.0 / 9.0) );
}


} // namespace cie::gl