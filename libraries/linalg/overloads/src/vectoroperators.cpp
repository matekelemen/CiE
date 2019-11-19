#include "../inc/vectoroperators.hpp"


DoubleVector operator+(const DoubleVector& vector, double scalar)
{
    DoubleVector result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [scalar](auto vectorIt) -> double
            {return vectorIt + scalar;}
        );
    
    return result;
}


DoubleVector operator+(double scalar, const DoubleVector& vector)
{
    return vector + scalar;
}


DoubleVector operator-(const DoubleVector& vector, double scalar)
{
    DoubleVector result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [scalar](auto vectorIt) -> double
            {return vectorIt - scalar;}
        );
    
    return result;
}


DoubleVector operator+(const DoubleVector& lhs, const DoubleVector& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");

    DoubleVector result(lhs.size());
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


DoubleVector operator-(const DoubleVector& lhs, const DoubleVector& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");

    DoubleVector result(lhs.size());
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


DoubleVector operator*(const DoubleVector& vector, double scalar)
{
    DoubleVector result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [scalar](auto vectorIt) -> double
            {return vectorIt * scalar;}
        );
    
    return result;
}


DoubleVector operator*(double scalar, const DoubleVector& vector)
{
    return vector * scalar;
}


DoubleVector operator/(const DoubleVector& vector, double scalar)
{
    if (abs(scalar)<1e-15)
        throw std::runtime_error("Division by 0!");
    else {
        DoubleVector result(vector);
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


double operator*(const DoubleVector& lhs, const DoubleVector& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");
    
    return std::inner_product( lhs.begin(), lhs.end(), rhs.begin(), 0.0 );
}