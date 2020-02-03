// --- CSG Includes ---
#include "../../libraries/CSG/trees/inc/spacetreenode.hpp"

// --- Internal Includes ---
#include "../inc/hilbert_determinant.hpp"
#include "../inc/complex.hpp"


// Define target function
const size_t dimension      = 2;
const size_t subdivision    = 11;
const size_t depth          = 10;

const double gamma          = 0.0;
const double omega          = 1.0;
const double omegaCritical  = 1.0;
const size_t N              = 10;

auto targetFunction = [=](const cie::DoubleArray<dimension>& parameters)
{   
    cie::mathieu::Complex determinant = cie::mathieu::hilbertDeterminant( 
                                            parameters[0],
                                            parameters[1],
                                            gamma,
                                            omega,
                                            omegaCritical,
                                            N);
    //double result = abs(determinant[0]) < abs(determinant[1]) ? determinant[0] : determinant[1];
    return determinant[0];
};


namespace cie {
namespace mathieu {


int main()
{
    csg::SpaceTreeNode<dimension,subdivision> root( {5.0,5.0}, 10.0 );

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