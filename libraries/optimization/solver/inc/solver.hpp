#ifndef OPTIMIZATION_SOLVER_HPP
#define OPTIMIZATION_SOLVER_HPP

namespace cie {
namespace opt {


class OptimizationMethod {
public:
    virtual void step() =0;
};


}
}

#endif