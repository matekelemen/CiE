#ifndef OPTIMIZATION_OBJECTIVE_SUBSTITUTE
#define OPTIMIZATION_OBJECTIVE_SUBSTITUTE

// --- Linalg Includes ---
#include "linalg/types.hpp"

// --- STL Includes ---
#include <utility>

namespace cie {
namespace opt {

template <size_t N>
class ObjectiveSubstitute {
public:
    ObjectiveSubstitute();
    ObjectiveSubstitute(const DoubleArray<N>& coefficients, const DoubleArray<N>& direction, const DoubleArray<N>& base);

    virtual void build(const std::vector<const DoubleArray<N>*>& points, const DoubleVector& values);
    virtual std::pair<DoubleArray<N>,double> minimum() const =0;
    virtual double operator()(double parameter) const;

    double getCoefficient(uint index) const;
    void setCoefficient(uint index, double coefficient);

    const DoubleArray<N>& getDirection() const;
    void setDirection(const DoubleArray<N>& direction);
    
protected:    
    DoubleVector _coefficients;
    DoubleArray<N> _direction;
    DoubleArray<N> _base;
};

} // namespace opt
}

#include "objectivesubstitute_impl.hpp"

#endif