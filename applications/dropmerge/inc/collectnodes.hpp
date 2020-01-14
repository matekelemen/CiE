#ifndef DROPMERGE_COLLECT_NODES_HPP
#define DROPMERGE_COLLECT_NODES_HPP

#include "dynamictree.hpp"

namespace cie {
namespace csg {


class TreeData
{
public:
    TreeData();
    std::vector<DoubleArray<N>>& centers() { return _centers; }
    DoubleVector& edgeLengths() { return _edgeLengths; }
    std::vector<int>& boundaries() { return _boundaries; }

    std::vector<DoubleArray<N>> _centers;
    DoubleVector                _edgeLengths;
    std::vector<int>            _boundaries;
};


int checkBoundary(const NTreeNode<N,M>& root);

TreeData collectNodes(const DynamicTree& root);

void collectNodesRecursive(     const NTreeNode<N,M>& root,
                                std::vector<DoubleArray<N>>& centers,
                                DoubleVector& edgeLengths,
                                std::vector<int>& boundaries);


}
}

#endif