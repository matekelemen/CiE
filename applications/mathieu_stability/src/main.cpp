// --- CSG Includes ---
#include "../../libraries/CSG/trees/inc/spacetreenode.hpp"

// --- Internal Includes ---
#include "../inc/hilbert_determinant.hpp"
#include "../inc/complex.hpp"


// Define target function
const size_t dimension      = 3;
const size_t subdivision    = 5;
const size_t depth          = 7;

const double GAMMA          = 0.2;
const double OMEGA          = 1.0;
//const double OMEGACRITICAL  = 0.0;
const size_t N              = 5;

auto targetFunction = [](const cie::DoubleArray<dimension>& parameters)
{   
    cie::mathieu::Complex determinant = cie::mathieu::hilbertDeterminant( 
                                            parameters[0],
                                            parameters[1],
                                            GAMMA,
                                            OMEGA,
                                            parameters[2]/10.0,
                                            N);
    double result = abs(determinant[0]) < abs(determinant[1]) ? determinant[0] : determinant[1];
    return result;
};


namespace cie {
namespace mathieu {


int main()
{
    csg::SpaceTreeNode<dimension,subdivision> root( {5.0,5.0,5.0}, 10.0 );

    root.evaluate(targetFunction);
    root.divide(targetFunction,depth);

    csg::writeSpaceTree<dimension,subdivision>(root, "mathieu_stability.csv");

    return 0;
}


}
}


int main()
{
    return cie::mathieu::main();
}