#include "octreeutils.hpp"

namespace csg {


Point64 getNodePoint(const Node& root, uint index) {
    std::array<uint,3> index3 = base3(index);
    return Point64(
        root.box(0,0) + index3[0]/2.0 * (root.box(1,0)-root.box(0,0)),
        root.box(0,1) + index3[1]/2.0 * (root.box(1,1)-root.box(0,1)),
        root.box(0,2) + index3[2]/2.0 * (root.box(1,2)-root.box(0,2))
    );
}


bool isBoundary(const NodeData& data){
    bool boundary = false;
    uint j=0;
    for (uint i=0;i<data.size();++i){
        if (data[i]) ++j;
        if (i>1 && j!=i+1) {
            boundary = true;
            break;
        }
    }
    return boundary;
}


void interiorPoints(const Node& root, std::vector<Point64>& points){
    if (root.children()[0] == nullptr){
        for (uint i=0; i<root.data().size();++i){
            if (root.data(i)){
                points.push_back(getNodePoint(root,i));
            }
        }
    }
    else{
        for (auto it=root.children().begin();it!=root.children().end();++it){
            if (*it!=nullptr) interiorPoints(*(*it),points);
        }
    }
}


std::pair<const Node&, uint> getNeighbour(const Node& root, uint index, uint axis, bool direction){
    // direction= true if positive, false if negative
    std::array<uint,3> index3 = base3(index);
    // Check if root has children
    if (root.children()[0]!=nullptr){
        // Check whether to stay within this Node
        if ((direction==true && index3[axis]<2) || (direction==false && index3[axis]>0)){
            // Call getNeighbour on a child
        }
        else{
            // Call getNeighbour on a sibling Node
        }
    }
    // Check if the neighbour is within the same Node
    if ((direction==true && index3[axis]<2) || (direction==false && index3[axis]>0)){
        if (direction==true) index3[axis]++;
        else index3[axis]--;
        return std::make_pair(
            root,
            index3[0]+3*index3[1]+9*index3[2]
            );
    }
    else{
        // Neighbouring Node is outside the current one
        //const Node* neighbour;
        if (root.parent()!=nullptr){
            // Check if it's a corner node (no ones in base3)
            std::array<uint,3> index3 = base3(index);
            if (index3[0]!=1 && index3[1]!=1 && index3[2]!=1){
                
            }
        }
        else {
            // No neighbour in this direction -> return an invalid index (27)
            return std::make_pair(root,27);
        }
        return std::make_pair(*(new Node),0);
    }
}





std::array<bool,3> base2(uint index){
    std::array<bool,3> output({false,false,false});
    if (index/4 == 1) {
        output[2]   = true;
        index       -= 4;
    }
    if (index/2 == 1){
        output[1]   = true;
        index       -= 2;
    }
    if (index == 1) output[0] = true;

    return output;
}

std::array<uint,3> base3(uint index){
    std::array<uint,3> output({0,0,0});
    uint exp;
    for (uint i=0;i<3;++i){
        exp         = uintPow(3,2-i);
        output[2-i] = index / exp;
        index       -= exp*output[2-i];
    }

    return output;
}


uint uintPow(uint base, uint exponent){
    if (exponent==0) return 1;
    if (exponent==1) return base;
    uint output = uintPow(base,exponent/2);
    if (exponent%2 == 0) return output*output;
    else return base*output*output;
}


}