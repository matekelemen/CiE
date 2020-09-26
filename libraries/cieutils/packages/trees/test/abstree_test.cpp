// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "cieutils/packages/trees/inc/abstree.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>
#include <iostream>

namespace cie::utils {

struct TestTree;
typedef std::shared_ptr<TestTree> TestTreePtr;

struct TestTree : public AbsTree<std::vector,TestTree>
{
    TestTree(Size level) : AbsTree<std::vector,TestTree>(level) {}
};


TEST_CASE( "AbsTree", "[trees]" )
{
    TestTree root(0);

    // Create function that splits nodes in two new ones
    // if a specified depth has not yet been reached
    Size targetDepth = 5;
    Size counter = 0;
    auto split = [targetDepth,&counter]( TestTree* node ) -> bool
    {
        if (node->level() < targetDepth)
        {
            ++counter;
            node->children().push_back( std::make_shared<TestTree>(node->level()+1) );
            node->children().push_back( std::make_shared<TestTree>(node->level()+1) );
            return true;
        }
        return false;
    };

    REQUIRE_NOTHROW( root.visit(split) );
    CHECK( counter == targetDepth );
}


}