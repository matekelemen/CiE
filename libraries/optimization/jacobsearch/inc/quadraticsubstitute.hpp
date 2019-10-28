#ifndef OPTIMIZATION_QUADRATIC_SUBSTITUTE
#define OPTIMIZATION_QUADRATIC_SUBSTITUTE

#include "objectivesubstitute.hpp"
#include <utility>

namespace opt {

class QuadraticSubstitute : public ObjectiveSubstitute {
public:
    QuadraticSubstitute(std::vector<const DoubleVector*>& points, const DoubleVector& values);

    void build(std::vector<const DoubleVector*>& points, const DoubleVector& values) override;
    std::pair<DoubleVector,double> minimum() const override;
    double operator()(double parameter) const override;
};

}

#endif