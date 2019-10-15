#include "catch.hpp"
#include "octree_volumenode.hpp"


namespace csg {

TEST_CASE("VolumeNode - locate"){
    // Point set
    std::vector<Point64> set;
    for (uint i=0;i<11;++i){
        set.push_back(Point64(i/10.0, i/10.0, i/10.0));
    }
    // Find point
    for (uint i=0; i<set.size(); ++i){

        VolumeNode* root = new VolumeNode;
        root->setBoundingBox({Point64(0.0,0.0,0.0),Point64(1.0,1.0,1.0)});
        root->setPoints(set);

        Point64* target = &set[i];
        const VolumeNode* location = root->locate(target, 1e-5);

        REQUIRE( location!=nullptr );
        for (uint j=0; j<3; ++j){
            CHECK( (location->box(0,j)<=(*target)[j] && (*target)[j]<=location->box(1,j)) );
        }

        delete root;
    }
    // Point outside
    VolumeNode* root = new VolumeNode;
    root->setBoundingBox({Point64(0.0,0.0,0.0),Point64(1.0,1.0,1.0)});
    root->setPoints(set);
    Point64 target(2.0,0.5,0.5);
    CHECK( root->locate(&target, 1e-5)==nullptr );
}


TEST_CASE("VolumeNode - findContainingNode"){
    std::vector<Point64> set;
    for (uint i=0;i<11;++i){
        set.push_back(Point64(i/10.0, i/10.0, i/10.0));
    }

    VolumeNode* root = new VolumeNode;
    root->setBoundingBox({Point64(0.0,0.0,0.0),Point64(1.0,1.0,1.0)});
    root->setPoints(set);

    const VolumeNode* location1 = root->locate(&set[4], 1e-5);
    const VolumeNode* location2 = root->findContainingNode(&set[4]);

    CHECK( &(*location2) == &(*location1) );
}


TEST_CASE("VolumeNode - divide"){

    std::vector<Point64> set;
    for (uint i=0;i<11;++i){
        set.push_back(Point64(i/10.0, i/10.0, i/10.0));
    }

    VolumeNode* root = new VolumeNode;
    root->setBoundingBox({Point64(0.0,0.0,0.0),Point64(1.0,1.0,1.0)});
    root->setPoints(set);

    root->divide(0,10,2);
}

}