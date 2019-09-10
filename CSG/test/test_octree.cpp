#include "catch.hpp"
#include "octree.hpp"
#include "box.hpp"
#include "sphere.hpp"

namespace csg {


TEST_CASE("Base converters"){
    // Base 2
    uint test  = 7;
    std::array<bool,3> test2 = base2(test);
    for (uint i=0; i<3; ++i){
        CHECK( test2[i] == true );
    }

    test    = 3;
    test2   = base2(test);
    CHECK( test2[0] == true );
    CHECK( test2[1] == true );
    CHECK( test2[2] == false );

    // Base 3
    test  = 23;
    std::array<uint,3> test3 = base3(test);
    CHECK( test3[0] == 2 );
    CHECK( test3[1] == 1 );
    CHECK( test3[2] == 2 );

    test    = 13;
    test3   = base3(test);
    CHECK( test3[0] == 1 );
    CHECK( test3[1] == 1 );
    CHECK( test3[2] == 1 );
}


TEST_CASE("Interior points"){
    // Create test boxes
    Box* bigBox     = new Box(Point64({-1.0,-1.0,-1.0}),Point64({1.0,1.0,1.0}));
    Box* smallBox   = new Box(Point64({-0.1,-0.1,-0.1}),Point64({0.1,0.1,0.1}));

    // Create root node
    Node* root = new Node;

    // Test for the big box
    root->setGeometry(bigBox);
    root->setBoundingBox({Point64({-0.5,-0.5,-0.5}), Point64({0.5,0.5,0.5})});
    root->evaluateAll();
    for (uint i=0; i<root->data().size(); ++i){
        CHECK( root->data(i) == true );
    }

    // Test for the small box
    root->setGeometry(smallBox);
    root->evaluateAll();
    for (uint i=0; i<root->data().size(); ++i){
        if (i!=12) CHECK( root->data(i) == true );
        else CHECK( root->data(i) == true );
    }

    std::cout << root->box(0,0) << " " << root->box(0,1) << "\n";
    std::cout << root->box(1,0) << " " << root->box(1,1) << "\n";
    std::cout << root->box(2,0) << " " << root->box(2,1) << "\n";

    std::vector<Point64> smallPoints;
    interiorPoints(*root,smallPoints);
    REQUIRE( smallPoints.size() == 1 );
    CHECK( smallPoints[0][0] == Approx(0.0) );
    CHECK( smallPoints[0][1] == Approx(0.0) );
    CHECK( smallPoints[0][2] == Approx(0.0) );
}


TEST_CASE("Octree"){
    // Create geometry
    CSGObject* geometry     = new Sphere();

    // Create root node
    Node* root = new Node();
    root->setGeometry(geometry);
    root->setBoundingBox({Point64({-1.0,-1.0,-1.0}),{1.0,1.0,1.0}});

    // Evaluate all points
    root->evaluateAll();

    // Start division
    uint minLevel = 1;
    uint maxLevel = 3;
    root->divide(minLevel,maxLevel);

    // Collect interior points
    std::vector<Point64> geomPoints;
    csg::interiorPoints(*root, geomPoints);

    // Write interior points to file
    csg::writePointVector("octree_test.csv",geomPoints);
}

}