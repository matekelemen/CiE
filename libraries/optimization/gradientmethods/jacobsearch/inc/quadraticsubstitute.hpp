#ifndef OPTIMIZATION_QUADRATIC_SUBSTITUTE
#define OPTIMIZATION_QUADRATIC_SUBSTITUTE

#include "objectivesubstitute.hpp"
#include <utility>

namespace cie {
namespace opt {

template <size_t N>
class QuadraticSubstitute : public ObjectiveSubstitute<N> {
public:
    QuadraticSubstitute(std::vector<const DoubleArray<N>*>& points, const DoubleVector& values);

    void build(std::vector<const DoubleArray<N>*>& points, const DoubleVector& values) override;
    std::pair<DoubleArray<N>,double> minimum() const override;
    double operator()(double parameter) const override;
};

} // namespace opt
}

#include "quadraticsubstitute_impl.hpp"

#endif