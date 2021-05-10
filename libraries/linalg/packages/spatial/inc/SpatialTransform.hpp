#ifndef CIE_LINALG_SPATIAL_TRANSFORM_HPP
#define CIE_LINALG_SPATIAL_TRANSFORM_HPP

// --- External Includes ---
#include "Eigen/Dense"

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- Internal Includes ---
#include "linalg/packages/spatial/inc/Quaternion.hpp"
#include "linalg/packages/types/inc/MatrixWrapper.hpp"

// --- STL Includes ---
#include <array>
#include <ostream>


namespace cie::linalg {


template <concepts::NumericType NT>
class SpatialTransform
{
public:
    template <Size Dimension>
    using vector = std::array<NT,Dimension>;

protected:
    using matrix_type = Eigen::Matrix<NT,4,4>;
    using matrix_wrapper = MatrixWrapper<matrix_type>;

public:
    SpatialTransform( SpatialTransform<NT>&& r_rhs ) = default;

    explicit SpatialTransform( const SpatialTransform<NT>& r_rhs ) = default;

    /// Identity by default
    SpatialTransform();

    SpatialTransform<NT>& operator=( const SpatialTransform<NT>& r_rhs ) = default;

    vector<4> transform( const vector<4>& r_vector ) const;

    vector<3> transform( const vector<3>& r_vector ) const;

    void transformInPlace( SpatialTransform<NT>& r_transform ) const;

    const matrix_wrapper& transformationMatrix() const;

public:
    /// 3D translation
    static SpatialTransform<NT> makeTranslation( const vector<3>& r_vector );

    /// 2D translation in the xy-plane
    static SpatialTransform<NT> makeTranslation( const vector<2>& r_vector );

    /// 3D rotation
    static SpatialTransform<NT> makeRotation( const Quaternion<NT>& r_quaternion,
                                              const vector<3>& r_reference = {0, 0, 0} );

    /// 3D rotation
    static SpatialTransform<NT> makeRotation( const vector<3>& r_axis,
                                              NT angle,
                                              const vector<3>& r_reference = {0, 0, 0} );

    /// 2D rotation in the xy-plane
    static SpatialTransform<NT> makeRotation( NT angle,
                                              const vector<2>& r_reference = {0, 0} );

private:
    matrix_type& matrix();

    const matrix_type& matrix() const;

protected:
    matrix_wrapper _transformationMatrix;
};


template <concepts::NumericType NT>
std::ostream& operator<<( std::ostream& r_stream, const SpatialTransform<NT>& r_transform );


} // namespace cie::linalg

#include "linalg/packages/spatial/impl/SpatialTransform_impl.hpp"

#endif