#include "csg.hpp"
#include <iostream>

namespace csg{

void CSGTest(){
    Point64 testPoint(1.5,0.5,0.5);

    Box* box1 = new Box( Point64({0.0,0.0,0.0}), Point64({1.0,1.0,1.0}) );
    Box* box2 = new Box( Point64({-1.0,-1.0,-1.0}), Point64({2.0,2.0,2.0}) );

    std::cout << box1->isInside( testPoint ) << std::endl;
    std::cout << box2->isInside( testPoint ) << std::endl;
    
    CSGOperator* geom1    = new CSGUnion(box1,box2);
    CSGOperator* geom2    = new CSGSubtract(box1,box2);
    CSGOperator* geom3    = new CSGSubtract(box2,box1);
    CSGOperator* geom4    = new CSGIntersect(box1,box2);

    std::cout << geom1->isInside( testPoint ) << std::endl;
    std::cout << geom2->isInside( testPoint ) << std::endl;
    std::cout << geom3->isInside( testPoint ) << std::endl;
    std::cout << geom4->isInside( testPoint ) << std::endl;

}


void octreeTest(){
    std::cout << "Create geometry\n";
    CSGObject* geometry     = new Sphere();
    std::cout << "Create root node\n";
    Node* root = new Node();
    std::cout << "Set geometry\n";
    root->setGeometry(geometry);
    std::cout << "Set bounding box\n";
    root->setBoundingBox({Point64({-1.0,-1.0,-1.0}),{1.0,1.0,1.0}});
    std::cout << "Evaluate all nodes\n";
    root->evaluateAll();
    std::cout << "Call divide on root\n";
    root->divide(1,3);
    std::cout << "Get interior points\n";
    std::vector<Point64> geomPoints;
    csg::interiorPoints(*root, geomPoints);
    std::cout << "Exit octreeTest()\n";
}


}

int main(){
    std::cout << "Main begin\n";
    csg::CSGTest();
    std::cout << "After CSGTest()\n";
    csg::octreeTest();
    std::cout << "After octreeTest()\n";
    return 0;
}