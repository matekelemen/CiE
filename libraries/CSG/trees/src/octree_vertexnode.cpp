#include "../inc/octree_vertexnode.hpp"


namespace cie {
namespace csg {


std::array<uint,8> cubeCornerIndicesO2 = {
    0,1,3,4,
    9,10,12,13};


VertexNode::VertexNode() :
    Node(),
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
        false,false,false})
{
}


VertexNode::VertexNode(const VertexNode& parent, uint index) :
    Node(parent,index),
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
        false,false,false})
    
{
    std::array<bool,3> boolIndex = base2(index);
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


VertexNode::~VertexNode(){
    for (auto it=children_.begin();it!=children_.end();++it){
        if (*it!=nullptr) delete *it;
        else std::cout << "csg::VertexNode::~VertexNode() attempted to delete nullptr child\n";
    }
}


void VertexNode::divide(uint minLevel, uint maxLevel){
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
        for (auto it=children_.begin();it!=children_.end();++it){
            *it = new VertexNode(*this, std::distance(children_.begin(),it));
            (*it)->divide(minLevel,maxLevel);
        }
    }
}


void VertexNode::evaluateAll(){
    for (uint i=0;i<27;++i){
        data_[i] = geometry_->isInside(getNodePoint(*this,i));
    }
}


const NodeData& VertexNode::data() const {return data_;}


bool VertexNode::data(uint index) const {return data_[index];}



void VertexNode::interiorPoints(std::vector<Point64>& points){
    if (children_[0] == nullptr){
        for (uint i=0; i<data_.size();++i){
            if (data_[i]){
                points.push_back(getNodePoint(*this,i));
            }
        }
    }
    else{
        for (auto it=children_.begin();it!=children_.end();++it){
            if (*it!=nullptr) (static_cast<VertexNode*>(*it))->interiorPoints(points);
        }
    }
}


void VertexNode::setGeometry(CSGObject* geometry) {geometry_ = geometry;}



// ----------------------------------------------------------------------
// FREE-FLOATING FUNCTIONS
// ----------------------------------------------------------------------

Point64 getNodePoint(const VertexNode& root, uint index) {
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


std::pair<const VertexNode&, uint> getNeighbour(const VertexNode& root, uint index, uint axis, bool direction){
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
            base10(index3)
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
        return std::make_pair(*(new VertexNode),0);
    }
}




}
}