#include "../inc/vectoroperators.hpp"

namespace linalg {
    

Vector operator+(const Vector& vector, double scalar)
{
    Vector result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [scalar](auto vectorIt) -> double
            {return vectorIt + scalar;}
        );
    
    return result;
}


Vector operator+(double scalar, const Vector& vector)
{
    return vector + scalar;
}


Vector operator-(const Vector& vector, double scalar)
{
    Vector result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [scalar](auto vectorIt) -> double
            {return vectorIt - scalar;}
        );
    
    return result;
}


Vector operator+(const Vector& lhs, const Vector& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");

    Vector result(lhs.size());
    std::transform( 
        lhs.begin(), 
        lhs.end(), 
        rhs.begin(), 
        result.begin(), 
        [](auto lhsIt, auto rhsIt) -> double
            {return (lhsIt) + (rhsIt);}
        );
    
    return result;
}


Vector operator-(const Vector& lhs, const Vector& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");

    Vector result(lhs.size());
    std::transform( 
        lhs.begin(), 
        lhs.end(), 
        rhs.begin(), 
        result.begin(), 
        [](auto lhsIt, auto rhsIt) -> double
            {return (lhsIt) - (rhsIt);}
        );

    return result;
}


Vector operator*(const Vector& vector, double scalar)
{
    Vector result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [scalar](auto vectorIt) -> double
            {return vectorIt * scalar;}
        );
    
    return result;
}


Vector operator*(double scalar, const Vector& vector)
{
    return vector * scalar;
}


Vector operator/(const Vector& vector, double scalar)
{
    if (abs(scalar)<1e-15)
        throw std::runtime_error("Division by 0!");
    else {
        Vector result(vector);
        std::transform( 
            vector.begin(), 
            vector.end(), 
            result.begin(), 
            [scalar](auto vectorIt) -> double
                {return vectorIt / scalar;}
            );
        
        return result;
    }
}


double operator*(const Vector& lhs, const Vector& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");
    
    return std::inner_product( lhs.begin(), lhs.end(), rhs.begin(), 0.0 );
}


}