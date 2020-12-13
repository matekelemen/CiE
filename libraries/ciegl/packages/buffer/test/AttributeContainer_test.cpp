// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AttributeContainer.hpp"
#include "ciegl/packages/buffer/inc/AbsVertex.hpp"


namespace cie::gl {


TEST_CASE( "AttributeContainer", "[buffer]" )
{
    CIE_TEST_CASE_INIT( "AttributeContainer" )

    REQUIRE_NOTHROW( AttributeContainerPtr( new AttributeContainer ) );

    AttributeContainerPtr attributeContainer(
        new AttributeContainer
    );

    // TODO
}


} // namespace cie::gl