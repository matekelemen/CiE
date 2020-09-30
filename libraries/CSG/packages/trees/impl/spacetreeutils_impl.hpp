#ifndef CSG_NTREE_UTILITIES_IMPL_HPP
#define CSG_NTREE_UTILITIES_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>
#include <string>


namespace cie::csg {


template <size_t N, size_t M>
void writeSpaceTree(const SpaceTreeNode<N,M>& node, std::ostream& file)
{
    std::string buffer = std::to_string(node.edgeLength()) + ',';

    std::for_each(  node.center().begin(),
                    node.center().end(),
                    [&buffer] (auto value) { buffer += std::to_string(value) + ','; } );

    for (auto it=node.data().begin(); it!=node.data().end()-1; ++it) 
        buffer += std::to_string(*it) + ',';

    buffer += std::to_string(*(node.data().end()-1)) + '\n';

    file << buffer;
}


template <size_t N, size_t M>
void writeSpaceTree(const SpaceTreeNode<N,M>& node, const std::string& filename)
{
    //std::ofstream file(filename, std::ios::out | std::ios::binary);
    std::ofstream file(filename);
    if (file.is_open())
    {
        // Write header
        std::string buffer  = "length,";
        size_t dataSize     = intPow(M,N);

        for (size_t i=0; i<N; ++i)  
            buffer += "center" + std::to_string(i) + ',';

        for (size_t i=0; i<dataSize-1; ++i) 
            buffer += "value" + std::to_string(i) + ',';

        buffer += "value" + std::to_string(dataSize-1) + '\n';

        file << buffer;

        // Write data
        node.write(file);
    }
    else
        CIE_THROW( std::runtime_error, "Unable to write to " + filename )

    file.close();
}


// Collecting nodes --------------------------------------------------
template <class ContainerType>
bool uniformSigns(const ContainerType& data)
{
    bool result = data[0] > 0.0;
    for (auto it=data.begin()+1; it!=data.end(); ++it)
        if ( ((*it) > 0.0) != result) return false;
    return true;
}


template <size_t N, size_t M>
std::vector<const SpaceTreeNode<N,M>*> boundaryNodes(const SpaceTreeNode<N,M>& root)
{
    std::vector<const SpaceTreeNode<N,M>*> nodes;
    boundaryNodes<N,M>(root,nodes);
    return nodes;
}


template <size_t N, size_t M>
void boundaryNodes(     const SpaceTreeNode<N,M>& root,
                        std::vector<const SpaceTreeNode<N,M>*>& nodes )
{
    if (uniformSigns(root.data()))
        nodes.push_back(&root);

    for (auto it=root.children().begin(); it!=root.children().end(); ++it)
        if (*it!=nullptr)
            boundaryNodes(**it, nodes);
}


} // namespace cie::csg


#endif