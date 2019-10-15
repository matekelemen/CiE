#include "octree_volumenode.hpp"

namespace csg {

VolumeNode::VolumeNode() :
    Node()
{
}


VolumeNode::VolumeNode(const VolumeNode& parent, uint index) :
    Node(parent, index)
{
    activePoints_.reserve(parent.activePoints_.size());
    for (auto it=parent.activePoints_.begin(); it!=parent.activePoints_.end();++it){
        if (isInside(**it)) activePoints_.push_back(*it);
    }
    activePoints_.shrink_to_fit();
}


VolumeNode::~VolumeNode(){
    for (auto it=children_.begin(); it!=children_.end(); ++it){
        if ((*it)!=nullptr) {
            delete *it;
            *it = nullptr;
        }
    }
}


// Locate a point within the node.
// This function creates one child at max, in the octant where the point is
const VolumeNode* VolumeNode::locate(const Point64* target, double tolerance){
    // Check tolerance
    if (tolerance <= 0){
        throw std::runtime_error("Invalid tolerance in VolumeNode::locate");
    }
    // Check whether the point is within bounds
    if (isInside(*target)){
        // Check whether the tolerance is reached
        if (box_[1][0]-box_[0][0] < tolerance || activePoints_.size()==1){
            return this;
        } 
        else {

            // Determine which part the point is in
            std::array<bool,3> index2 = {false,false,false};
            for (uint i=0; i<target->size(); ++i){
                if ((*target)[i] > box_[0][i]+0.5*(box_[1][i]-box_[0][i])){
                    index2[i] = true;
                }
            }
            uint index = base10(index2);
            // Construct the correct child and call locate on it
            VolumeNode* child = new VolumeNode(*this,index);
            children_[index] = child;
            const VolumeNode* location = (dynamic_cast<VolumeNode*>(children_[index]))->locate(target,tolerance);
            
            return location;
            
        }
    }
    else return nullptr;
}


// Partition the point set
void VolumeNode::divide(uint minLevel, uint maxLevel){
    // Check whether the max level is reached
    if (level_ >= maxLevel || activePoints_.size()<=1) return;
    // Divide if the point list is not empty
    if (activePoints_.size()!=0 || level_<minLevel){
        for (uint i=0; i<children_.size(); ++i){
            children_[i] = new VolumeNode(*this, i);
            static_cast<VolumeNode*>(children_[i])->divide(minLevel, maxLevel);
        }
    }
}


void VolumeNode::divide(uint minLevel, uint maxLevel, uint maxPoints){
    if (activePoints_.size()>maxPoints) divide(minLevel,maxLevel);
}


const VolumeNode* VolumeNode::findContainingNode(const Point64* target) const {
    if (isInside(*target)){
        for (auto it=children_.begin(); it!=children_.end(); ++it){
            if (*it != nullptr){
                VolumeNode* node = static_cast<VolumeNode*>(*it);
                if ( node->isInside(*target) ){
                    return node->findContainingNode(target);
                }
            }
        }
    }
    // Unsuccessful search
    else return nullptr;
    // This is the leaf node containing the target
    return this;
}


bool VolumeNode::isInside(const Point64& point) const {
    for (uint i=0; i<point.size(); ++i){
        if ( sgn(point[i]-box_[0][i]) == sgn(point[i]-box_[1][i]) ) {
            if ( point[i]-box_[0][i] == 0.0 || point[i]-box_[1][i]==0.0 ) return true;
            else return false;
        }
    }
    return true;
}


std::vector<const Point64*>& VolumeNode::activePoints(){
    return activePoints_;
}


const std::vector<const Point64*>& VolumeNode::activePoints() const {
    return activePoints_;
}


void VolumeNode::setPoints(const std::vector<Point64>& set){
    activePoints_.clear();
    activePoints_.reserve(set.size());
    for (uint i=0; i<set.size(); ++i){
        activePoints_.push_back(&set[i]);
    }
}



}