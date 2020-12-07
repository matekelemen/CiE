// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

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