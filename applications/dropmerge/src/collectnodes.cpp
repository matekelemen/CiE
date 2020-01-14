#include "../inc/collectnodes.hpp"

namespace cie {
namespace csg {


TreeData::TreeData() :
    _centers(),
    _edgeLengths(),
    _boundaries()
{
}


int checkBoundary(const NTreeNode<N,M>& root)
{
    bool result = root.data()[0] > 0.0;

    if (root.children()[0] == nullptr)
    {
        for (auto it=root.data().begin()+1; it!=root.data().end(); ++it)
        {
            if ( ((*it)>0.0) != result )
            {
                return 1;
            }
        }
    }

    return 0;
}


TreeData collectNodes(const DynamicTree& root)
{
    TreeData data;
    collectNodesRecursive(  root,
                            data._centers,
                            data._edgeLengths,
                            data._boundaries    );
    return data;
}


void collectNodesRecursive(     const NTreeNode<N,M>& root,
                                std::vector<DoubleArray<N>>& centers,
                                DoubleVector& edgeLengths,
                                std::vector<int>& boundaries)
{
    centers.push_back( root.center() );
    edgeLengths.push_back( root.edgeLength() );
    boundaries.push_back(checkBoundary(root));

    for (auto it=root.children().begin(); it!=root.children().end(); ++it)
    {
        if (*it!=nullptr)
            collectNodesRecursive(  **it,
                                    centers,
                                    edgeLengths,
                                    boundaries);
        else break;
    }
}


}
}