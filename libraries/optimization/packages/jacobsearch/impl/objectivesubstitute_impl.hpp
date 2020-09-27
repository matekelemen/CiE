
namespace cie::opt {


template <size_t N>
ObjectiveSubstitute<N>::ObjectiveSubstitute() :
    _coefficients(0)
{
	_direction.fill(0.0);
	_base.fill(0.0);
}


template <size_t N>
ObjectiveSubstitute<N>::ObjectiveSubstitute(const DoubleArray<N>& coefficients, const DoubleArray<N>& direction, const DoubleArray<N>& base) :
    _coefficients(coefficients),
    _direction(direction),
    _base(base)
{
}


template <size_t N>
void ObjectiveSubstitute<N>::build(const std::vector<const DoubleArray<N>*>& points, const DoubleVector& values) 
{
    _coefficients = DoubleVector(points.size(),0.0);
}


template <size_t N>
double ObjectiveSubstitute<N>::operator()(double parameter) const 
{
    return 0.0;
}


template <size_t N>
double ObjectiveSubstitute<N>::getCoefficient(Size index) const 
{
    return _coefficients[index];
}


template <size_t N>
void ObjectiveSubstitute<N>::setCoefficient(Size index, double coefficient) 
{
    _coefficients[index] = coefficient;
}


template <size_t N>
const DoubleArray<N>& ObjectiveSubstitute<N>::getDirection() const 
{
    return _direction;
}


template <size_t N>
void ObjectiveSubstitute<N>::setDirection(const DoubleArray<N>& direction) 
{
    _direction = direction;
}


} // namespace cie::opt