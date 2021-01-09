// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shapes/inc/Arrow.hpp"
#include "ciegl/packages/buffer/inc/Vertex3.hpp"


namespace cie::gl {


CIE_TEST_CASE( "Arrow", "[shapes]" )
{
    CIE_TEST_CASE_INIT( "Arrow" )

    using VertexType = Vertex3;
    using ArrowType  = Arrow<VertexType>;

    auto p_attributes = AttributeContainerPtr(
        new AttributeContainer
    );

    CIE_TEST_CHECK_NOTHROW( ArrowType( p_attributes, {0.0,0.0,0.0}, {1.0,0.0,0.0} ) );
    ArrowType arrow( p_attributes,
                     {0.0, 0.0, 0.0},
                     {1.0, 1.0, 1.0} );

    auto indices = arrow.indices();
    Size counter = 0;
    for ( auto index : indices )
        std::cout << index << (++counter%3==0 ? "\n" : " ");
}


} // namespace cie::gl