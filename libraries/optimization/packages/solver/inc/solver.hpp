#ifndef OPTIMIZATION_SOLVER_HPP
#define OPTIMIZATION_SOLVER_HPP

namespace cie::opt {


class OptimizationMethod {
public:
    virtual void step() =0;
};


} // namespace cie::opt

#endif