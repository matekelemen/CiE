#include "doublechromosome.hpp"

namespace opt {

DoubleChromosome::DoubleChromosome() :
    Chromosome(),
    _data({}),
    _deviation(1.0)
{
}

DoubleChromosome::DoubleChromosome(const DoubleVector& data) :
    Chromosome(),
    _data(data),
    _deviation(1.0)
{
}

DoubleChromosome::DoubleChromosome(const DoubleVector& data, double deviation) :
    Chromosome(),
    _data(data),
    _deviation(deviation)
{
}

DoubleChromosome::DoubleChromosome(const DoubleChromosome& toCopy) :
    Chromosome(toCopy._objective),
    _data(toCopy._data),
    _deviation(toCopy._deviation)
{
}

DoubleChromosome DoubleChromosome::operator=(const DoubleChromosome& toCopy){
    return DoubleChromosome(toCopy._data, toCopy._deviation, toCopy._objective);
}

double& DoubleChromosome::operator[](uint index) {
    return _data[index];
}

const double& DoubleChromosome::operator[](uint index) const {
    return _data[index];
}


void DoubleChromosome::setData(const DoubleVector& newData) {
    _data = newData;
}

const DoubleVector& DoubleChromosome::getData() const {
    return _data;
}

void DoubleChromosome::setDeviation(double newDeviation) {
    _deviation = newDeviation;
}

double DoubleChromosome::getDeviation() const {
    return _deviation;
}

}