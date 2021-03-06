#ifndef CIE_LINALG_MATRIX_HPP
#define CIE_LINALG_MATRIX_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "linalg/packages/types/inc/vectortypes.hpp"

// --- STL Includes ---
#include <vector>
#include <iostream>
#include <array>
#include <memory>

namespace cie::linalg {


template <concepts::NumericType ValueType = Double>
class Matrix
{
public:
    using value_type     = ValueType;
    using container_type = std::vector<value_type>;

    using size_type      = typename container_type::size_type;
    using iterator       = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

public:
    explicit Matrix( );

    Matrix( Size size1, Size size2 );
    Matrix( Size size1, Size size2, ValueType value );

    Matrix( const std::vector<ValueType>& rowMajorData, Size size1 );
    Matrix( const std::vector<DoubleVector>& vectorOfRows );

    Size size1( ) const;
    Size size2( ) const;
    Size size() const;

    std::array<Size, 2> sizes( ) const;

    ValueType& operator()( Size i, Size j );
    ValueType operator()( Size i, Size j ) const;
    ValueType& operator[]( Size i );
    ValueType operator[]( Size i ) const;

	const std::vector<ValueType>& data() const { return data_; }

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    void transpose();

private:
    void checkIndices(Size i, Size j) const;

    Size                    size1_, size2_;
    std::vector<ValueType>  data_;
    bool                    _transpose;
}; // class Matrix



template <class ValueType>
using MatrixPtr = std::shared_ptr<Matrix<ValueType>>;



} // namespace cie::linalg

#include "linalg/packages/types/impl/matrix_impl.hpp"

#endif