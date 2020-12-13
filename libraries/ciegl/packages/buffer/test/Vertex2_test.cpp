// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/Vertex2.hpp"


namespace cie::gl {


CIE_TEST_CASE( "Vertex2", "[buffer]" )
{
    CIE_CIE_TEST_CASE_INIT( "Vertex2" )

    auto p_attributes = AttributeContainerPtr( new AttributeContainer );

    REQUIRE_NOTHROW( makeVertex<Vertex2>( p_attributes ) );

    Vertex2Ptr p_vertex;
    REQUIRE_NOTHROW( p_vertex = makeVertex<Vertex2>( p_attributes ) );

    CHECK_NOTHROW( p_vertex->x() = 1.0 );
    CHECK_NOTHROW( p_vertex->y() = 2.0 );
    CHECK( p_vertex->x() == Approx(1.0) );
    CHECK( p_vertex->y() == Approx(2.0) );

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

    CHECK( p_vertex->x() == Approx(3.0 / 4.0) );
    CHECK( p_vertex->y() == Approx(5.0 / 6.0) );
}


} // namespace cie::gl