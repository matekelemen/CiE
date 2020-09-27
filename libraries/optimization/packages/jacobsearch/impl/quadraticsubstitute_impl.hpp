
// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/types/inc/types.hpp"

namespace cie::opt {


template<size_t N>
QuadraticSubstitute<N>::QuadraticSubstitute(const std::vector<const DoubleArray<N>*>& points, const DoubleVector& values) :
    ObjectiveSubstitute<N>()
{
    build(points, values);
}


template<size_t N>
void QuadraticSubstitute<N>::build(const std::vector<const DoubleArray<N>*>& points, const DoubleVector& values) {
    // Check sizes
    if (points.size()!=3 || values.size()!=3)
        CIE_THROW( std::runtime_error, "Invalid nuumber of data sites!" )

    if (points[0]->size()!=points[1]->size() || points[1]->size()!=points[2]->size())
        CIE_THROW( OutOfRangeException, "Inconsistent point sizes!" )
    
    auto pts = points;
    auto vls = values;

    // Determine main component
    size_t mainComponent    = 0;
    double tolerance        = 1e-16;
    for (size_t i=0; i<N; ++i)
    {
        if ( std::abs( (*pts[0])[i] - (*pts[1])[i] ) > tolerance && std::abs( (*pts[0])[i] - (*pts[2])[i] ) > tolerance )
        {
            mainComponent = i;
            break;
        }
    }

    // Sort points
    if ( ( (*pts[1])[mainComponent]>(*pts[0])[mainComponent] )  !=  ( (*pts[2])[mainComponent]>(*pts[1])[mainComponent] ) ){
        auto temp1  = pts[2];
        pts[2]      = pts[0];
        pts[0]      = temp1;

        auto temp2  = vls[2];
        vls[2]      = vls[0];
        vls[0]      = temp2;
    }

    // Set base as first point
    this->_base = DoubleArray<N>(*pts[0]);

    // Compute direction
    for (Size i=0; i<this->_direction.size(); ++i){
        this->_direction[i] = (*pts[2])[i]-(*pts[0])[i];
    }

    // Compute coefficients (internal parameters: [0,ratio,1])
    double ratio    = ( (*pts[2])[mainComponent]-(*pts[0])[mainComponent] );

    if ( std::abs(ratio)<tolerance )
        CIE_THROW( std::runtime_error, "Duplicate end sample point!" )

    ratio           = ( (*pts[1])[mainComponent]-(*pts[0])[mainComponent] ) / ratio;
    double ratio2   = ratio*ratio;

    if ( std::abs(ratio)<tolerance || std::abs(ratio-1.0)<tolerance )
        CIE_THROW( std::runtime_error, "Duplicate middle sample point!" )

    this->_coefficients = {
        vls[2],
        ( vls[2]-vls[1]/ratio2-vls[0]*(1-1/ratio2) ) / ( 1-1/ratio ),
        ( vls[2]-vls[1]/ratio-vls[0]*(1-1/ratio) ) / ( 1-ratio )
    };
}


template<size_t N>
std::pair<DoubleArray<N>,double> QuadraticSubstitute<N>::minimum() const {

    // Check minimum requirements
    if (std::abs(this->_coefficients[2])<1e-15)
        CIE_THROW( std::runtime_error, "Substitute function is linear!" )

    if (this->_coefficients[2]<0.0)
        CIE_THROW( std::runtime_error, "Substitute function does not have a minimum!" )

    // Compute parameter at minimum
    double minParam     = -this->_coefficients[1]/2/this->_coefficients[2];

    // Get coordinates at minimum
    DoubleArray<N> minPos(this->_base);
    for (Size i=0; i<minPos.size(); ++i){
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



} // namespace cie::opt