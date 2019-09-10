#include "octree.hpp"

namespace csg {


std::array<uint,8> cubeCornerIndicesO2 = {
    0,1,3,4,
    9,10,12,13};


Node::Node() :
    geometry_(nullptr),
    data_({
        false,false,false,false,false,
        false,false,false,false,false,
        false,false,false,false,false,
        false,false,false,false,false,
        false,false,false,false,false}),
    box_({Point64(),Point64()}),
    children_({
        nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr}),
    level_(0)
{
}


Node::Node(const Node& parent, uint index) :
    geometry_(parent.geometry_),
    children_({
        nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr}),
    level_(parent.level_+1)
{
    std::array<bool,3> boolIndex = base2(index);
    // Bounding box
    for (uint i=0;i<3;++i){
        box_[0][i] = parent.box_[0][i] + (boolIndex[i] ? 0.5 : 0.0)*(parent.box_[1][i]-parent.box_[0][i]);
        box_[1][i] = parent.box_[0][i] + (boolIndex[i] ? 1.0 : 0.5)*(parent.box_[1][i]-parent.box_[0][i]);
    }
    // Data - inherited from parent
    uint baseIndex = boolIndex[0]+3*boolIndex[1]+9*boolIndex[2];
    for (uint i=0;i<cubeCornerIndicesO2.size();++i){
        data_[2*i] = parent.data_[i+baseIndex];
    }
    // Data - new
    baseIndex = 0;
    std::array<uint,3> i3;
    Point64 testPoint;

    for (uint i=0;i<27;++i){
        if (i!=cubeCornerIndicesO2[baseIndex]){
            i3  = base3(i);
            for (uint j=0;j<3;++j){
                testPoint[j] = box_[0][j] + i3[j]/2.0*(box_[1][j]-box_[0][j]);
            }
            data_[i] = geometry_->isInside(testPoint);
        }
        else ++baseIndex;
    }

}


Node::~Node(){
    for (auto it=children_.begin();it!=children_.end();++it){
        if (*it!=nullptr) delete *it;
        else std::cout << "csg::Node::~Node() attempted to delete nullptr child\n";
    }
}


void Node::divide(uint minLevel, uint maxLevel){
    // Check level
    if (level_ >= maxLevel) return;
    // Check data
    bool divide = false;
    if (level_ < minLevel) divide=true;
    else{
        if (isBoundary(data_)) divide=true;
    }
    // Divide
    if (divide){
        for (uint i=0;i<children_.size();++i){
            children_[i] = new Node(*this, i);
            children_[i]->divide(minLevel,maxLevel);
        }
    }
}


void Node::evaluateAll(){
    std::array<uint,3> i3;
    Point64 testPoint;
    for (uint i=0;i<27;++i){
        i3  = base3(i);
        for (uint j=0;j<3;++j){
            testPoint[j] = box_[0][j] + i3[j]/2.0*(box_[1][j]-box_[0][j]);
        }
        data_[i] = geometry_->isInside(testPoint);
    }
}


const NodeData& Node::data() const {return data_;}


bool Node::data(uint index) const {return data_[index];}


const BoundingBox& Node::box() const {return box_;}


double Node::box(uint i, uint j) const {return box_[i][j];}


const std::array<Node*,8> Node::children() const {return children_;}


uint Node::level() const {return level_;}


void Node::setGeometry(CSGObject* geometry) {geometry_ = geometry;}


void Node::setBoundingBox(const BoundingBox& box) {box_=box;}




// -----------------------------------------------------------
// FREE-FLOATING FUNCTIONS
// -----------------------------------------------------------
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
    std::array<uint,3> i3;
    for (uint i=0; i<root.data().size();++i){
        if (root.data(i)){
            i3 = base3(i);
            points.push_back(Point64({
                root.box(0,0) + i3[0]/2*(root.box(1,0)-root.box(0,0)),
                root.box(0,1) + i3[1]/2*(root.box(1,1)-root.box(0,1)),
                root.box(0,2) + i3[2]/2*(root.box(1,2)-root.box(0,2))
            }));
        }
    }
    for (auto it=root.children().begin();it!=root.children().end();++it){
        if (*it!=nullptr) interiorPoints(*(*it),points);
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