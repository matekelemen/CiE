#ifndef MATHIEU_STABILITY_COMPLEX_HPP
#define MATHIEU_STABILITY_COMPLEX_HPP

#include <cstddef>

namespace cie {
namespace mathieu {

class Complex
{
public:
    Complex();
    Complex(double real, double imag);
    Complex( const Complex& copy );
    Complex operator=( const Complex& copy );

    double& operator[]( size_t index );
    const double& operator[](size_t index) const;

    void operator-=( double scalar );

    friend Complex operator*( int scalar, const Complex& complex );
    friend Complex operator*( double scalar, const Complex& complex );
    friend Complex operator*( const Complex& lhs, const Complex& rhs );
    friend Complex operator+( const Complex& lhs, const Complex& rhs );
    friend Complex operator-(const Complex& lhs, const Complex& rhs);

private:
    double _real;
    double _imag;

};

}
}

#endif