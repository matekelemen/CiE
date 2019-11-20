namespace opt {


template <size_t N>
ObjectiveSubstitute<N>::ObjectiveSubstitute()
{
	_coefficients.fill(0.0);
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
void ObjectiveSubstitute<N>::build(std::vector<const DoubleArray<N>*>& points, const DoubleArray<N>& values) 
{
    _coefficients = DoubleArray<N>( values.size(), 0.0 );
}


template <size_t N>
double ObjectiveSubstitute<N>::operator()(double parameter) const 
{
    return 0.0;
}


template <size_t N>
double ObjectiveSubstitute<N>::getCoefficient(uint index) const 
{
    return _coefficients[index];
}


template <size_t N>
void ObjectiveSubstitute<N>::setCoefficient(uint index, double coefficient) 
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


}