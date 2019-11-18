#ifndef OPTIMIZATION_OBJECTIVE_SUBSTITUTE
#define OPTIMIZATION_OBJECTIVE_SUBSTITUTE

#include "linalgtypes.hpp"
#include <utility>

namespace opt {

class ObjectiveSubstitute {
public:
    ObjectiveSubstitute();
    ObjectiveSubstitute(const DoubleVector& coefficients, const DoubleVector& direction, const DoubleVector& base);

    virtual void build(std::vector<const DoubleVector*>& points, const DoubleVector& values);
    virtual std::pair<DoubleVector,double> minimum() const =0;
    virtual double operator()(double parameter) const;

    double getCoefficient(uint index) const;
    void setCoefficient(uint index, double coefficient);

    const DoubleVector& getDirection() const;
    void setDirection(const DoubleVector& direction);
    
protected:
    ObjectiveSubstitute(uint numberOfCoefficients, uint numberOfVariables);

    DoubleVector _coefficients;
    DoubleVector _direction;
    DoubleVector _base;
};

}

#endif