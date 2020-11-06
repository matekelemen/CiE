#include "../inc/complex.hpp"
#include <string>
#include <stdexcept>

namespace cie {
namespace mathieu {


Complex::Complex() :
    _real(0.0),
    _imag(0.0)
{
}


Complex::Complex( double real, double imag ) :
    _real(real),
    _imag(imag)
{
}


Complex::Complex( const Complex& copy ) :
    _real(copy._real),
    _imag(copy._imag)
{
}


Complex Complex::operator=( const Complex& copy )
{
    return Complex(*this);
}


double& Complex::operator[]( size_t index )
{
    if (index==0)
        return _real;
    else if (index==1)
        return _imag;
    else
        throw std::runtime_error("Invalid index for Complex::operator[] (" + std::to_string(index) + ")" );
}


const double& Complex::operator[](size_t index) const
{
    if (index == 0)
        return _real;
    else if (index == 1)
        return _imag;
    else
        throw std::runtime_error("Invalid index for Complex::operator[] (" + std::to_string(index) + ")");
}


void Complex::operator-=( double scalar )
{
    _real -= scalar;
}


Complex operator*(const Complex& lhs, const Complex& rhs)
{
    return Complex( lhs._real*rhs._real - lhs._imag*rhs._imag,
                    lhs._real*rhs._imag + lhs._imag*rhs._real );
}


Complex operator+(const Complex& lhs, const Complex& rhs)
{
    return Complex( lhs._real + rhs._real ,
                    lhs._imag + rhs._imag);
}


Complex operator-(const Complex& lhs, const Complex& rhs)
{
    return Complex( lhs._real - rhs._real,
                    lhs._imag - rhs._imag);
}


Complex operator*(int scalar, const Complex& complex)
{
    return Complex( scalar*complex[0], scalar*complex[1] );
}


Complex operator*(double scalar, const Complex& complex)
{
    return Complex(scalar*complex[0], scalar*complex[1]);
}


}
}