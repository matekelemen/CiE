#include "../inc/octree_node.hpp"

namespace csg {


Node::Node() :
    box_({Point64(),Point64()}),
    children_({
        nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr}),
    level_(0),
    parent_(nullptr)
{
}


Node::Node(const Node& parent, uint index) : 
    children_({
        nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr}),
    level_(parent.level_+1),
    parent_(&parent)
{
    std::array<bool,3> boolIndex = base2(index);
    // Bounding box
    for (uint i=0;i<3;++i){
        box_[0][i] = parent.box_[0][i] + (boolIndex[i] ? 0.5 : 0.0)*(parent.box_[1][i]-parent.box_[0][i]);
        box_[1][i] = parent.box_[0][i] + (boolIndex[i] ? 1.0 : 0.5)*(parent.box_[1][i]-parent.box_[0][i]);
    }
}


const BoundingBox& Node::box() const {return box_;}


double Node::box(uint i, uint j) const {return box_[i][j];}


const std::array<Node*,8>& Node::children() const {return children_;}


uint Node::level() const {return level_;}


const Node* Node::parent() const {return parent_;}


void Node::setBoundingBox(const BoundingBox& bbox) {
    for (uint i=0;i<box_.size();++i){
        for (uint j=0;j<box_[i].size();++j){
            box_[i][j] = bbox[i][j];
        }
    }
}




}