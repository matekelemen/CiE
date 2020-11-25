// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "CSG/packages/partitioning/inc/AABBoxNode.hpp"


namespace cie::csg {


class TestAABBoxNodeValueType : public AbsBoundableObject<2,Double>
{
    void computeBoundingBox_impl( typename TestAABBoxNodeValueType::bounding_box& r_box ) override
    {}
};


TEST_CASE( "AABBoxNode", "[partitioning]" )
{
    CIE_TEST_CASE_INIT( "AABBoxNode" )

    using Node = AABBoxNode<TestAABBoxNodeValueType>;
}


} // namespace cie::csg