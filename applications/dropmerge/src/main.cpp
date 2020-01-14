#include "mergefunction.hpp"
#include <stdlib.h>

namespace cie {
namespace csg {

const uint8_t dimension     = 3;
const uint8_t subdivision   = 7;
const uint8_t depth         = 6;

int main(std::function<double(const DoubleArray<dimension>&)> target, double offset = 0.25)
{
    NTreeNode<dimension,subdivision> root( {0.0,0.0}, 4.0 );
    root.evaluate(target);
    root.divide(target,depth);

    writeNTree<dimension,subdivision>(root, "dropmerge.csv");

    std::string filename    = "install/pycsg/dropmerge.py";
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
    if (argc>0)
        offset = std::atof(argv[1]);

    auto target =  [=](const cie::DoubleArray<cie::csg::dimension>& point) 
        {return cie::csg::exponentialMergeFunction<cie::csg::dimension>(point,offset);};


    return cie::csg::main(target, offset);
}