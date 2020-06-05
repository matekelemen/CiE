#ifndef LINALG_VECTOR_OPERATORS_IMPL_HPP
#define LINALG_VECTOR_OPERATORS_IMPL_HPP

// --- STD Includes ---
#include <algorithm>
#include <numeric>

namespace cie {

// ---------------------------------------------------------
// VECTOR - SCALAR OPERATORS
// ---------------------------------------------------------
template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator+( const ArrayType& vector, const ScalarType& scalar )
{
    ArrayType result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component + scalar;}
        );
    return result;
}


template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator+( const ScalarType& scalar, const ArrayType& vector )
{
    return vector + scalar;
}


template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator-( const ArrayType& vector, const ScalarType& scalar )
{
    ArrayType result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component - scalar;}
        );
    return result;
}


template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator-( const ScalarType& scalar, const ArrayType& vector )
{
    return vector - scalar;
}


template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator*( const ArrayType& vector, const ScalarType& scalar )
{
    ArrayType result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component * scalar;}
        );
    return result;
}


template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator/( const ArrayType& vector, const ScalarType& scalar )
{
    ArrayType result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component / scalar;}
        );
    return result;
}


template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator*( const ScalarType& scalar, const ArrayType& vector )
{
    return vector * scalar;
}


// ---------------------------------------------------------
// VECTOR - VECTOR OPERATORS
// ---------------------------------------------------------
template <concepts::NumericContainer ArrayType>
ArrayType operator+( const ArrayType& lhs, const ArrayType& rhs )
{
    typedef typename ArrayType::value_type ValueType;
    ArrayType result;
    std::transform( 
        lhs.begin(), 
        lhs.end(), 
        rhs.begin(), 
        result.begin(), 
        [](const ValueType& lhsComponent, const ValueType& rhsComponent) -> ValueType
            {return lhsComponent + rhsComponent;}
        );
    
    return result;
}


template <concepts::NumericContainer ArrayType>
ArrayType operator-( const ArrayType& lhs, const ArrayType& rhs )
{
    typedef typename ArrayType::value_type ValueType;
    ArrayType result;
    std::transform( 
        lhs.begin(), 
        lhs.end(), 
        rhs.begin(), 
        result.begin(), 
        [](const ValueType& lhsComponent, const ValueType& rhsComponent) -> ValueType
            {return lhsComponent - rhsComponent;}
        );
    
    return result;
}


template <concepts::NumericContainer ArrayType>
typename ArrayType::value_type operator*( const ArrayType& lhs, const ArrayType& rhs )
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");
    
    return std::inner_product( lhs.begin(), lhs.end(), rhs.begin(), 0.0 );
}

}

#endif