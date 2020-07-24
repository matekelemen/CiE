#ifndef CIE_LINALG_VECTOR_OPERATORS_IMPL_HPP
#define CIE_LINALG_VECTOR_OPERATORS_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- STL Includes ---
#include <algorithm>
#include <numeric>


// ---------------------------------------------------------
// VECTOR - SCALAR OPERATORS
// ---------------------------------------------------------

template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator+( const ArrayType& vector, const ScalarType& scalar )
{
    ArrayType result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component + scalar;}
        );
    return result;
}


template <cie::Size N, cie::concepts::NumericType ScalarType>
std::array<ScalarType,N> operator+( const std::array<ScalarType,N>& vector, const ScalarType& scalar )
{
    std::array<ScalarType,N> result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component + scalar;}
        );
    return result;
}


template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator+( const ScalarType& scalar, const ArrayType& vector )
{
    return vector + scalar;
}


template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator-( const ArrayType& vector, const ScalarType& scalar )
{
    ArrayType result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component - scalar;}
        );
    return result;
}


template <cie::Size N, cie::concepts::NumericType ScalarType>
std::array<ScalarType,N> operator-( const std::array<ScalarType,N>& vector, const ScalarType& scalar )
{
    std::array<ScalarType,N> result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component - scalar;}
        );
    return result;
}


template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator-( const ScalarType& scalar, const ArrayType& vector )
{
    return vector - scalar;
}


template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator*( const ArrayType& vector, const ScalarType& scalar )
{
    ArrayType result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component * scalar;}
        );
    return result;
}


template <cie::Size N, cie::concepts::NumericType ScalarType>
std::array<ScalarType,N> operator*( const std::array<ScalarType,N>& vector, const ScalarType& scalar )
{
    std::array<ScalarType,N> result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component * scalar;}
        );
    return result;
}


template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator/( const ArrayType& vector, const ScalarType& scalar )
{
    CIE_DIVISION_BY_ZERO_ASSERT( scalar!=0, "operator/(vector,scalar)" )

    ArrayType result(vector.size());
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component / scalar;}
        );
    return result;
}


template <cie::Size N, cie::concepts::NumericType ScalarType>
std::array<ScalarType,N> operator/( const std::array<ScalarType,N>& vector, const ScalarType& scalar )
{
    CIE_DIVISION_BY_ZERO_ASSERT( scalar!=0, "operator/(std::array,scalar)" )

    std::array<ScalarType,N> result;
    std::transform( 
        vector.begin(), 
        vector.end(), 
        result.begin(), 
        [&scalar](const auto& component) -> ScalarType
            {return component / scalar;}
        );
    return result;
}


template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator*( const ScalarType& scalar, const ArrayType& vector )
{
    return vector * scalar;
}


// ---------------------------------------------------------
// VECTOR - VECTOR OPERATORS
// ---------------------------------------------------------
template <cie::concepts::NumericContainer ArrayType>
ArrayType operator+( const ArrayType& lhs, const ArrayType& rhs )
{
    CIE_OUT_OF_RANGE_ASSERT( lhs.size() == rhs.size(), "operator+(Container,Container)" )

    typedef typename ArrayType::value_type ValueType;
    ArrayType result(lhs.size());
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


template <cie::concepts::NumericType ValueType, cie::Size N>
std::array<ValueType,N> operator+( const std::array<ValueType,N>& lhs, const std::array<ValueType,N>& rhs )
{
    std::array<ValueType,N> result;
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


template <cie::concepts::NumericContainer ArrayType>
ArrayType operator-( const ArrayType& lhs, const ArrayType& rhs )
{
    CIE_OUT_OF_RANGE_ASSERT( lhs.size() == rhs.size(), "operator+(Container,Container)" )

    typedef typename ArrayType::value_type ValueType;
    ArrayType result(lhs.size());
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


template <cie::concepts::NumericType ValueType, cie::Size N>
std::array<ValueType,N> operator-( const std::array<ValueType,N>& lhs, const std::array<ValueType,N>& rhs )
{
    std::array<ValueType,N> result;
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


template <cie::concepts::NumericContainer ArrayType>
typename ArrayType::value_type operator*( const ArrayType& lhs, const ArrayType& rhs )
{
    if (lhs.size() != rhs.size())
        throw std::runtime_error("Inconsistent vector sizes!");
    
    return std::inner_product( lhs.begin(), lhs.end(), rhs.begin(), 0.0 );
}


#endif