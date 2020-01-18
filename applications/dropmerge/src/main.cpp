#include "mergefunction.hpp"
#include <stdlib.h>

namespace cie {
namespace csg {

const size_t dimension     = 2;
const size_t subdivision   = 5;
const size_t depth         = 10;

int main(std::function<double(const DoubleArray<dimension>&)> target, double offset = 0.0)
{
    NTreeNode<dimension,subdivision> root( {0.0,0.0}, 4.0 );
    root.evaluate(target);
    root.divide(target,depth);

    writeNTree<dimension,subdivision>(root, "dropmerge.csv");

    std::string filename    = "install/applications/dropmerge/dropmerge_csv.py";
    std::string command     = "python3 ";
    command                 += filename;
    system(command.c_str());

    return 0;
}

}
}


int main(int argc, char *argv[])
{
    // Parse argument
    double offset = 0.0;
    if (argc>1)
        offset = std::atof(argv[1]);

    auto target =  [=](const cie::DoubleArray<cie::csg::dimension>& point) 
        {return cie::csg::exponentialMergeFunction<cie::csg::dimension>(point,offset);};


    return cie::csg::main(target, offset);
}