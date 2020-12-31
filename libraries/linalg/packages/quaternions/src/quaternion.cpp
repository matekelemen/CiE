// --- Internal Includes ---
#include "linalg/packages/quaternions/inc/quaternion.hpp"
#include "linalg/packages/overloads/inc/vectoroperators.hpp"


namespace cie::linalg {


Quaternion::Quaternion() :
    _coefficients( {1.0, 0.0, 0.0, 0.0} )
{
}


Quaternion::Quaternion( const DoubleArray<4>& coefficients ) :
    _coefficients( coefficients )
{
}


Quaternion::Quaternion( const Quaternion& copy ) :
    _coefficients( copy._coefficients )
{
}


void Quaternion::operator=(const Quaternion& copy)
{
    _coefficients = copy._coefficients;
}


const DoubleArray<4>& Quaternion::coefficients() const
{
    return _coefficients;
}


double Quaternion::operator[]( size_t index ) const
{
    return _coefficients[index];
}


DoubleArray<4>::const_iterator Quaternion::begin() const
{
    return _coefficients.begin();
}


DoubleArray<4>::const_iterator Quaternion::end() const
{
    return _coefficients.end();
}


Quaternion operator+( const Quaternion& lhs, const Quaternion& rhs )
{
    return Quaternion( ::operator+(lhs.coefficients(), rhs.coefficients()) );
}


Quaternion operator-( const Quaternion& lhs, const Quaternion& rhs )
{
    return lhs + ((-1)*rhs);
}


Quaternion operator*( const Quaternion& lhs, const Quaternion& rhs )
{
    return Quaternion({
        lhs[0]*rhs[0] - lhs[1]*rhs[1] - lhs[2]*rhs[2] - lhs[3]*rhs[3],
        lhs[0]*rhs[1] + lhs[1]*rhs[0] + lhs[2]*rhs[3] - lhs[3]*rhs[2],
        lhs[0]*rhs[2] + lhs[2]*rhs[0] + lhs[3]*rhs[1] - lhs[1]*rhs[3],
        lhs[0]*rhs[3] + lhs[3]*rhs[0] + lhs[1]*rhs[2] - lhs[2]*rhs[1]
    });
}


Quaternion operator*( const Quaternion& quaternion, double scalar )
{
    return Quaternion( ::operator*(quaternion.coefficients(), scalar) );
}


Quaternion operator*( double scalar, const Quaternion& quaternion )
{
    return quaternion * scalar;
}


Quaternion conjugate( const Quaternion& quaternion )
{
    return Quaternion({
        quaternion[0],
        -quaternion[1],
        -quaternion[2],
        -quaternion[3]
    });
}



} // namespace cie::linalg