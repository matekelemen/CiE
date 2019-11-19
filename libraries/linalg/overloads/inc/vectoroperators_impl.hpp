#ifndef LINALG_VECTOR_OPERATORS_IMPL_HPP
#define LINALG_VECTOR_OPERATORS_IMPL_HPP


template <size_t N>
DoubleArray<N> operator+(const DoubleArray<N>& vector, double scalar)
{
    DoubleArray<N> result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [scalar](auto vectorIt) -> double
            {return vectorIt + scalar;}
        );
    
    return result;
}


template <size_t N>
DoubleArray<N> operator+(double scalar, const DoubleArray<N>& vector)
{
    return vector + scalar;
}


template <size_t N>
DoubleArray<N> operator-(const DoubleArray<N>& vector, double scalar)
{
    DoubleArray<N> result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [scalar](auto vectorIt) -> double
            {return vectorIt - scalar;}
        );
    
    return result;
}


template <size_t N>
DoubleArray<N> operator+(const DoubleArray<N>& lhs, const DoubleArray<N>& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");

    DoubleArray<N> result;
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


template <size_t N>
DoubleArray<N> operator-(const DoubleArray<N>& lhs, const DoubleArray<N>& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");

    DoubleArray<N> result;
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


template <size_t N>
DoubleArray<N> operator*(const DoubleArray<N>& vector, double scalar)
{
    DoubleArray<N> result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [scalar](auto vectorIt) -> double
            {return vectorIt * scalar;}
        );
    
    return result;
}


template <size_t N>
DoubleArray<N> operator*(double scalar, const DoubleArray<N>& vector)
{
    return vector * scalar;
}


template <size_t N>
DoubleArray<N> operator/(const DoubleArray<N>& vector, double scalar)
{
    if (abs(scalar)<1e-15)
        throw std::runtime_error("Division by 0!");
    else {
        DoubleArray<N> result(vector);
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


template <size_t N>
double operator*(const DoubleArray<N>& lhs, const DoubleArray<N>& rhs)
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");
    
    return std::inner_product( lhs.begin(), lhs.end(), rhs.begin(), 0.0 );
}


#endif