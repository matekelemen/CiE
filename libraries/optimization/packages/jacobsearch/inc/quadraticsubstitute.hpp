#ifndef OPTIMIZATION_QUADRATIC_SUBSTITUTE
#define OPTIMIZATION_QUADRATIC_SUBSTITUTE

// --- Internal Includes ---
#include "optimization/packages/jacobsearch/inc/objectivesubstitute.hpp"

// --- STL Includes ---
#include <utility>

namespace cie::opt {

template <size_t N>
class QuadraticSubstitute : public ObjectiveSubstitute<N> {
public:
    QuadraticSubstitute(const std::vector<const DoubleArray<N>*>& points, const DoubleVector& values);

    void build(const std::vector<const DoubleArray<N>*>& points, const DoubleVector& values) override;
    std::pair<DoubleArray<N>,double> minimum() const override;
    double operator()(double parameter) const override;
};

} // namespace cie::opt

#include "optimization/packages/jacobsearch/impl/quadraticsubstitute_impl.hpp"

#endif