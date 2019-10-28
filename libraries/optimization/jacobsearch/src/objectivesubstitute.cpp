#include "objectivesubstitute.hpp"

namespace opt {


ObjectiveSubstitute::ObjectiveSubstitute() :
    _coefficients(0),
    _direction(0)
{
}


ObjectiveSubstitute::ObjectiveSubstitute(const DoubleVector& coefficients, const DoubleVector& direction, const DoubleVector& base) :
    _coefficients(coefficients),
    _direction(direction),
    _base(base)
{
}


void ObjectiveSubstitute::build(std::vector<const DoubleVector*>& points, const DoubleVector& values) {
    _coefficients = DoubleVector( values.size(), 0.0 );
}


double ObjectiveSubstitute::operator()(double parameter) const {
    return 0.0;
}


double ObjectiveSubstitute::getCoefficient(uint index) const {
    return _coefficients[index];
}


void ObjectiveSubstitute::setCoefficient(uint index, double coefficient) {
    _coefficients[index] = coefficient;
}


const DoubleVector& ObjectiveSubstitute::getDirection() const {
    return _direction;
}


void ObjectiveSubstitute::setDirection(const DoubleVector& direction) {
    _direction = direction;
}


ObjectiveSubstitute::ObjectiveSubstitute(uint numberOfCoefficients, uint numberOfVariables) :
    _coefficients(numberOfCoefficients),
    _direction(numberOfVariables),
    _base(numberOfVariables)
{
}


}