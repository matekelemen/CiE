#ifndef CIE_FEM_UTILITIES_KERNEL_HPP
#define CIE_FEM_UTILITIES_KERNEL_HPP

// --- External Includes ---
#include "Eigen/Dense"

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- LinAlg Includes ---
#include "linalg/packages/types/inc/MatrixWrapper.hpp"
#include "linalg/packages/types/inc/matrix.hpp"

// --- STL Includes ---
#include <array>
#include <complex>



namespace cie::concepts {

template <class T>
concept EigenNumeric
= concepts::NumericType<T> && (
       std::is_same_v<T,float>
    || std::is_same_v<T,double>
    || std::is_same_v<T,std::complex<float>>
    || std::is_same_v<T,std::complex<double>> );

template <class T>
concept NotEigenNumeric
= concepts::NumericType<T> && !EigenNumeric<T>;

} // namespace cie::concepts::detail



namespace cie::fem {


template <Size Dimension, concepts::NumericType NT>
struct KernelBase
{
public:
    static const Size dimension = Dimension;
    using number_type           = NT;
    using point_type            = std::array<NT,Dimension>;
};


template <Size Dimension, concepts::NumericType NT>
struct Kernel : public KernelBase<Dimension,NT>
{
public:
    struct dense
    {
        template <Size NumberOfRows, Size NumberOfColumns>
        using matrix_type = linalg::MatrixWrapper<linalg::Matrix<NT>>;

        using square_matrix = matrix_type<Dimension,Dimension>;
    };
};


template <Size Dimension, concepts::EigenNumeric NT>
struct Kernel<Dimension,NT> : public KernelBase<Dimension,NT>
{
public:
    struct dense
    {
        template <Size NumberOfRows, Size NumberOfColumns>
        using matrix_type = linalg::MatrixWrapper<Eigen::Matrix<NT,NumberOfRows,NumberOfColumns>>;

        using square_matrix = matrix_type<Dimension,Dimension>;
    };
};


} // namespace cie::fem


#endif