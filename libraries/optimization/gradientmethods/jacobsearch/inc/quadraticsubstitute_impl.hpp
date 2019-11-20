

namespace cie {
namespace opt {


template<size_t N>
QuadraticSubstitute<N>::QuadraticSubstitute(std::vector<const DoubleArray<N>*>& points, const DoubleVector& values) :
    ObjectiveSubstitute<N>()
{
    build(points, values);
}


template<size_t N>
void QuadraticSubstitute<N>::build(std::vector<const DoubleArray<N>*>& points, const DoubleVector& values) {
    // Check sizes
    if (points.size()!=3 || values.size()!=3) 
        throw std::runtime_error("Invalid number of data sites!");

    if (points[0]->size()!=points[1]->size() || points[1]->size()!=points[2]->size())
        throw std::runtime_error("Inconsistent point sizes!");

    // Sort points
    if ( ( (*points[1])[0]>(*points[0])[0] )  !=  ( (*points[2])[0]>(*points[1])[0] ) ){
        auto temp = points[1];
        points[1] = points[0];
        points[0] = temp;
    }

    // Set base as first point
    this->_base = DoubleArray<N>(*points[0]);

    // Compute direction
    for (uint i=0; i<this->_direction.size(); ++i){
        this->_direction[i] = (*points[2])[i]-(*points[0])[i];
    }

    // Compute coefficients (internal parameters: [0,ratio,1])
    double ratio    = ( (*points[2])[0]-(*points[0])[0] );

    if ( std::abs(ratio)<1e-15 )
        throw std::runtime_error("Repeated end sample point!");

    ratio           = ( (*points[1])[0]-(*points[0])[0] ) / ratio;
    double ratio2   = ratio*ratio;

    if ( std::abs(ratio)<1e-15 || std::abs(ratio-1.0)<1e-15 )
        throw std::runtime_error("Repeated middle sample point!");

    this->_coefficients = {
        values[2],
        ( values[2]-values[1]/ratio2-values[0]*(1-1/ratio2) ) / ( 1-1/ratio ),
        ( values[2]-values[1]/ratio-values[0]*(1-1/ratio) ) / ( 1-ratio )
    };   
}


template<size_t N>
std::pair<DoubleArray<N>,double> QuadraticSubstitute<N>::minimum() const {

    // Check minimum requirements
    if (std::abs(this->_coefficients[2])<1e-15)
        throw std::runtime_error("Substitute function is linear!");

    if (this->_coefficients[2]<0.0)
        throw std::runtime_error("Substitute function does not have a minimum!");

    // Compute parameter at minimum
    double minParam     = -this->_coefficients[1]/2/this->_coefficients[2];

    // Get coordinates at minimum
    DoubleArray<N> minPos(this->_base);
    for (uint i=0; i<minPos.size(); ++i){
        minPos[i] += minParam*this->_direction[i];
    }
    
    // Return minimum coordinates and minimum value
    return std::make_pair<DoubleArray<N>, double> (
        DoubleArray<N>(minPos),
        (*this)(minParam)
    );
}


template<size_t N>
double QuadraticSubstitute<N>::operator()(double parameter) const {
    return this->_coefficients[0] + parameter * (this->_coefficients[1] + parameter*this->_coefficients[2]);
}



}
}