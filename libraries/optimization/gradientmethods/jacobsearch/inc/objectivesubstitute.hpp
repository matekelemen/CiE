#ifndef OPTIMIZATION_OBJECTIVE_SUBSTITUTE
#define OPTIMIZATION_OBJECTIVE_SUBSTITUTE

#include "linalgtypes.hpp"
#include <utility>

namespace opt {

template <size_t N>
class ObjectiveSubstitute {
public:
    ObjectiveSubstitute();
    ObjectiveSubstitute(const DoubleArray<N>& coefficients, const DoubleArray<N>& direction, const DoubleArray<N>& base);

    virtual void build(std::vector<const DoubleArray<N>*>& points, const DoubleArray<N>& values);
    virtual std::pair<DoubleArray<N>,double> minimum() const =0;
    virtual double operator()(double parameter) const;

    double getCoefficient(uint index) const;
    void setCoefficient(uint index, double coefficient);

    const DoubleArray<N>& getDirection() const;
    void setDirection(const DoubleArray<N>& direction);
    
protected:
    DoubleArray<N> _coefficients;
    DoubleArray<N> _direction;
    DoubleArray<N> _base;
};

} // namespace opt

#include "objectivesubstitute_impl.hpp"

#endif