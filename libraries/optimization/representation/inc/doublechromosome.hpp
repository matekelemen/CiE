#ifndef OPTIMIZATION_DOUBLECHROMOSOME_HPP
#define OPTIMIZATION_DOUBLECHROMOSOME_HPP

#include <memory>

#include "chromosome.hpp"

namespace opt {

class DoubleChromosome : public Chromosome {
public:
    DoubleChromosome();
    DoubleChromosome(const DoubleVector& data);
    DoubleChromosome(const DoubleVector& data, double deviation);
    DoubleChromosome(const DoubleChromosome& toCopy);

    DoubleChromosome operator=(const DoubleChromosome& toCopy);
    double& operator[](uint index);
    const double& operator[](uint index) const;

    void setData(const DoubleVector& newData);
    const DoubleVector& getData() const;
    void setDeviation(double newDeviation);
    double getDeviation() const;

protected:
    DoubleChromosome(const DoubleVector& data, double deviation, double objective);
    DoubleVector _data;
    double _deviation;
};

}

#endif