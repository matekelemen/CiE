#include "octree.hpp"

namespace csg {


std::array<uint,8> cubeCornerIndicesO2 = {
    0,1,3,4,
    9,10,12,13};


Node::Node() :
    geometry_(nullptr),
    data_({
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false}),
    box_({Point64(),Point64()}),
    children_({
        nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr}),
    level_(0)
{
}


Node::Node(const Node& parent, uint index) :
    geometry_(parent.geometry_),
        data_({
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false,
        false,false,false}),
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
    uint baseIndex = (uint)boolIndex[0]+3*(uint)boolIndex[1]+9*(uint)boolIndex[2];
    for (uint i=0;i<cubeCornerIndicesO2.size();++i){
        data_[2*i] = parent.data_[i+baseIndex];
    }
    // Data - new
    baseIndex = 0;
    for (uint i=0;i<data_.size();++i){
        if (i!=2*cubeCornerIndicesO2[baseIndex]){
            data_[i] = geometry_->isInside(getNodePoint(*this,i));
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
    bool div = false;
    if (level_ < minLevel) div=true;
    else{
        if (isBoundary(data_)) div=true;
    }
    // Divide
    if (div){
        for (uint i=0;i<children_.size();++i){
            children_[i] = new Node(*this, i);
            children_[i]->divide(minLevel,maxLevel);
        }
    }
}


void Node::evaluateAll(){
    for (uint i=0;i<27;++i){
        data_[i] = geometry_->isInside(getNodePoint(*this,i));
    }
}


const NodeData& Node::data() const {return data_;}


bool Node::data(uint index) const {return data_[index];}


const BoundingBox& Node::box() const {return box_;}


double Node::box(uint i, uint j) const {return box_[i][j];}


const std::array<Node*,8>& Node::children() const {return children_;}


uint Node::level() const {return level_;}


void Node::setGeometry(CSGObject* geometry) {geometry_ = geometry;}


void Node::setBoundingBox(const BoundingBox& bbox) {
    for (uint i=0;i<box_.size();++i){
        for (uint j=0;j<box_[i].size();++j){
            box_[i][j] = bbox[i][j];
        }
    }
}



}