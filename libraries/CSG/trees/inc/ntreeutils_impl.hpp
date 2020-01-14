#ifndef CSG_NTREE_UTILITIES_IMPL_HPP
#define CSG_NTREE_UTILITIES_IMPL_HPP

#include <string>

namespace cie {
namespace csg {


template <uint8_t M>
void baseN(size_t base_10, uint8_t base, UInt8Array<M>& base_N)
{
    for (int i = 0; i < M; ++i)
    {
        if (base_10 == 0)
        {
            base_N[i]   = 0;
        }
        else
        {
            base_N[i]   = base_10 % base;
            base_10     /= base;
        }
    }
    if (base_10 != 0)
        throw std::overflow_error("Error converting to base " + std::to_string(base));
}


template <uint8_t M>
UInt8Array<M> baseN(size_t base_10, uint8_t base)
{
    UInt8Array<M> result;
    baseN<M>(base_10, base, result);
    return result;
}


template <uint8_t M>
size_t base10(const UInt8Array<M>& base_N, uint8_t base)
{
    size_t power = 1;
    size_t base_10 = 0;
    for (uint8_t i = 0; i < M; ++i)
    {
        base_10 += power*base_N[i];
        power *= base;
    }
    return base_10;
}


template <uint8_t N, uint8_t M>
void writeNTree(const NTreeNode<N,M>& node, std::ostream& file)
{
    std::string buffer = std::to_string(node.edgeLength()) + ',';
    for (auto it=node.center().begin(); it!=node.center().end(); ++it) buffer += std::to_string(*it) + ',';
    for (auto it=node.data().begin(); it!=node.data().end()-1; ++it) buffer += std::to_string(*it) + ',';
    buffer += std::to_string(*(node.data().end()-1)) + '\n';
    file << buffer;
}


template <uint8_t N, uint8_t M>
void writeNTree(const NTreeNode<N,M>& node, const std::string& filename)
{
    //std::ofstream file(filename, std::ios::out | std::ios::binary);
    std::ofstream file(filename);
    if (file.is_open())
    {
        // Write header
        std::string buffer = "length,";
        int dataSize    = intPow(M,N);

        for (int i=0; i<N; ++i)  buffer += "center" + std::to_string(i) + ',';
        for (int i=0; i<dataSize-1; ++i) buffer += "value" + std::to_string(i) + ',';

        buffer += 'v' + std::to_string(dataSize-1) + '\n';

        file << buffer;

        // Write data
        node.write(file);
    }
    else throw std::runtime_error("Unable to write to " + filename);

    file.close();
}


// Collecting nodes --------------------------------------------------
template <uint8_t N, uint8_t M>
std::vector<const NTreeNode<N,M>*> boundaryNodes(const NTreeNode<N,M>& root)
{
    std::vector<const NTreeNode<N,M>*> nodes;
    boundaryNodes<N,M>(root,nodes);
    return nodes;
}


template <uint8_t N, uint8_t M>
void boundaryNodes(     const NTreeNode<N,M>& root,
                        std::vector<const NTreeNode<N,M>*>& nodes )
{
    if (uniformSigns(root.data()))
        nodes.push_back(&root);

    for (auto it=root.children().begin(); it!=root.children().end(); ++it)
        if (*it!=nullptr)
            boundaryNodes(**it, nodes);
}


}
}


#endif