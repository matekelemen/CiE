// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/partitioning/inc/AABBoxNode.hpp"

// --- STL Inlcudes ---
#include <deque>
#include <memory>
#include <iostream>


namespace cie::csg {


class TestAABBoxNodeObjectType final :
    public AbsBoundableObject<2,Double>,
    public AABBox<2,Double>
{
public:
    TestAABBoxNodeObjectType( const typename TestAABBoxNodeObjectType::point_type& r_base,
                              const typename TestAABBoxNodeObjectType::point_type& r_lengths ) :
        AbsBoundableObject<2,Double>(),
        AABBox<2,Double>( r_base, r_lengths ) {}
private:
    void computeBoundingBox_impl( typename TestAABBoxNodeObjectType::bounding_box& r_box ) override
    { r_box = *this; }
};


CIE_TEST_CASE( "AABBoxNode", "[partitioning]" )
{
    CIE_TEST_CASE_INIT( "AABBoxNode" )

    using Object = TestAABBoxNodeObjectType;
    using Node   = AABBoxNode<Object>;

    using ObjectPtr = std::shared_ptr<Object>;
    using NodePtr   = std::shared_ptr<Node>;

    // Generate objects
    std::deque<ObjectPtr> objects;

    Size numberOfCellsPerDimension = 5;

    for ( Size i=0; i<numberOfCellsPerDimension; ++i )
        for ( Size j=0; j<numberOfCellsPerDimension; ++j )
        {
            auto p_object = ObjectPtr( new Object(
                { Double(i)/numberOfCellsPerDimension, Double(j)/numberOfCellsPerDimension },
                { 1.0 / numberOfCellsPerDimension, 1.0 / numberOfCellsPerDimension }
            ) );
            objects.push_back( p_object );
        }

    // Generate root node
    Double delta = 0.0;
    auto p_root = NodePtr(
        new Node( { -delta, -delta },
                  { 1.0+2*delta, 1.0+2*delta },
                  typename Node::self_ptr() )
    );

    for ( auto& rp_object : objects )
        CHECK_NOTHROW( p_root->addObject(rp_object) );

    CHECK( p_root->objects().size() == numberOfCellsPerDimension*numberOfCellsPerDimension );

    // Partition
    const Size maxObjects = 3;
    const Size maxLevel   = 5;
    bool partitionSuccess;

    CHECK_NOTHROW( partitionSuccess = p_root->partition( maxObjects, maxLevel ) );
    CHECK( partitionSuccess );

    CHECK_NOTHROW( p_root->shrink() );

    // Check number of objects and maximum levels
    {
        auto nodeVisitFunction = [maxObjects,maxLevel]( Node* p_node ) -> bool
        {
            CHECK( p_node->level() <= maxLevel );

            if ( p_node->isLeaf() )
                CHECK( p_node->objects().size() <= maxObjects );

            return true;
        };

        CHECK_NOTHROW( p_root->visit(nodeVisitFunction) );
    }
}


} // namespace cie::csg