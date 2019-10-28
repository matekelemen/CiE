#include "quadraticsubstitute.hpp"

namespace opt {


QuadraticSubstitute::QuadraticSubstitute(std::vector<const DoubleVector*>& points, const DoubleVector& values) :
    ObjectiveSubstitute(points.size(), points[0]->size())
{
    build(points, values);
}


void QuadraticSubstitute::build(std::vector<const DoubleVector*>& points, const DoubleVector& values) {
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
    _base = DoubleVector(*points[0]);

    // Compute direction
    for (uint i=0; i<_direction.size(); ++i){
        _direction[i] = (*points[2])[i]-(*points[0])[i];
    }

    // Compute coefficients (internal parameters: [0,ratio,1])
    double ratio    = ( (*points[1])[0]-(*points[0])[0] ) / ( (*points[2])[0]-(*points[0])[0] );
    double ratio2   = ratio*ratio;
    _coefficients   = {
        values[2],
        ( values[2]-values[1]/ratio2-values[0]*(1-1/ratio2) ) / ( 1-1/ratio ),
        ( values[2]-values[1]/ratio-values[0]*(1-1/ratio) ) / ( 1-ratio )
    };   
}


std::pair<DoubleVector,double> QuadraticSubstitute::minimum() const {
    // Compute parameter at minimum
    double minParam     = -_coefficients[1]/2/_coefficients[2];

    // Get coordinates at minimum
    DoubleVector minPos(_base);
    for (uint i=0; i<minPos.size(); ++i){
        minPos[i] += minParam*_direction[i];
    }
    
    // Return minimum coordinates and minimum value
    return std::make_pair<DoubleVector, double> (
        DoubleVector(minPos),
        (*this)(minParam)
    );
}


double QuadraticSubstitute::operator()(double parameter) const {
    return _coefficients[0] + parameter * (_coefficients[1] + parameter*_coefficients[2]);
}



}