#include "doublechromosome.hpp"

namespace opt {

DoubleChromosome::DoubleChromosome() :
    Chromosome(),
    _data({})
{
}

DoubleChromosome::DoubleChromosome(const DoubleVector& data) :
    Chromosome(),
    _data(data)
{
}

DoubleChromosome::DoubleChromosome(const DoubleVector& data, double deviation) :
    Chromosome(),
    _data(data)
{
}

DoubleChromosome::DoubleChromosome(const DoubleChromosome& toCopy) :
    Chromosome(toCopy._objective),
    _data(toCopy._data)
{
}

DoubleChromosome DoubleChromosome::operator=(const DoubleChromosome& toCopy){
    return DoubleChromosome(toCopy._data, toCopy._objective);
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


}