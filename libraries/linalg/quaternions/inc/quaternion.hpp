#ifndef LINALG_QUATERNION_HPP
#define LINALG_QUATERNION_HPP

#include "../../types/inc/arraytypes.hpp"
#include <iterator>

namespace cie {
namespace linalg {


class Quaternion
{
public:
    Quaternion();
    explicit Quaternion( const DoubleArray<4>& coefficients );
    Quaternion( const Quaternion& copy );
    void operator=( const Quaternion& copy );

    const DoubleArray<4>& coefficients() const;
    double operator[](size_t index) const;

    DoubleArray<4>::const_iterator begin() const;
    DoubleArray<4>::const_iterator end() const;


    friend Quaternion operator+( const Quaternion& lhs, const Quaternion& rhs );
    friend Quaternion operator-( const Quaternion& lhs, const Quaternion& rhs );
    friend Quaternion operator*( const Quaternion& lhs, const Quaternion& rhs );
    friend Quaternion operator*( const Quaternion& quaternion, double scalar );
    friend Quaternion operator*( double scalar, const Quaternion& quaternion );
    friend Quaternion conjugate( const Quaternion& quaternion );

private:
    DoubleArray<4>  _coefficients;
};


}
}

#endif