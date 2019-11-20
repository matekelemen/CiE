#ifndef LINALG_MATRIX_IMPL_HPP
#define LINALG_MATRIX_IMPL_HPP

/*
 * Although we are not using templates we implement trivial
 * functions inline if they are called very often, such that
 * the compiler can optimize these function calls. The conse-
 * quence is that when we change something in this file we
 * have to recompile everything that includes linalg.hpp.
 */

namespace cie {
namespace linalg
{


inline double& Matrix::operator()( size_t i, size_t j )
{
     if (!_transpose)
        return data_[i * size2_ + j];
    else
        return data_[j*size2_ + i];
}


inline double Matrix::operator()( size_t i, size_t j ) const
{
    if (!_transpose)
        return data_[i * size2_ + j];
    else
        return data_[j*size2_ + i];
}


inline double& Matrix::operator[]( size_t i )
{
    return data_[i];
}


inline double Matrix::operator[]( size_t i ) const
{
    return data_[i];
}


inline size_t Matrix::size1( ) const
{
    return size1_;
}


inline size_t Matrix::size2( ) const
{
    return size2_;
}


inline size_t Matrix::size() const
{
    return data_.size();
}


inline std::array<size_t, 2> Matrix::sizes( ) const
{
    return { size1_, size2_ };
}


inline void Matrix::transpose()
{
    size_t temp = size1_;
    size1_      = size2_;
    size2_      = temp;
    _transpose  = !_transpose;
}

} // namespace linalg
}

#endif