#ifndef CIE_LINALG_SPATIAL_TRANSFORM_IMPL_HPP
#define CIE_LINALG_SPATIAL_TRANSFORM_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "linalg/packages/overloads/inc/vectoroperators.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::linalg {


template <concepts::NumericType NT>
SpatialTransform<NT>::SpatialTransform()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->matrix().setIdentity();

    CIE_OUT_OF_RANGE_CHECK( this->_transformationMatrix.numberOfRows() == 4 )
    CIE_OUT_OF_RANGE_CHECK( this->_transformationMatrix.numberOfColumns() == 4 )

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
inline typename SpatialTransform<NT>::vector<4>
SpatialTransform<NT>::transform( const typename SpatialTransform<NT>::vector<4>& r_vector ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    Eigen::Map<Eigen::Matrix<const NT,4,1>> vector( &r_vector[0] );
    typename SpatialTransform<NT>::vector<4> output;

    auto tmp = (this->matrix() * vector).eval();
    std::copy(
        &tmp(0,0),
        (&tmp(0,0)) + 4,
        output.begin()
    );

    return output;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
inline typename SpatialTransform<NT>::vector<3>
SpatialTransform<NT>::transform( const typename SpatialTransform<NT>::vector<3>& r_vector ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename SpatialTransform<NT>::vector<3> output {0, 0, 0};

    for ( Size rowIndex=0; rowIndex<3; ++rowIndex )
    {
        NT& r_component = output[rowIndex];

        for ( Size columnIndex=0; columnIndex<3; ++columnIndex )
        {
            r_component += this->_transformationMatrix( rowIndex, columnIndex ) * r_vector[columnIndex];
        }

        r_component += this->_transformationMatrix( rowIndex, 3 );
    }

    return output;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
void
SpatialTransform<NT>::transformInPlace( SpatialTransform<NT>& r_transform ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    r_transform.matrix() = this->matrix() * r_transform.matrix();

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
SpatialTransform<NT>
SpatialTransform<NT>::makeTranslation( const typename SpatialTransform<NT>::vector<3>& r_vector )
{
    CIE_BEGIN_EXCEPTION_TRACING

    SpatialTransform<NT> transform;
    transform._transformationMatrix( 0, 3 ) = r_vector[0];
    transform._transformationMatrix( 1, 3 ) = r_vector[1];
    transform._transformationMatrix( 2, 3 ) = r_vector[2];

    return transform;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
inline SpatialTransform<NT>
SpatialTransform<NT>::makeTranslation( const typename SpatialTransform<NT>::vector<2>& r_vector )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return SpatialTransform<NT>::makeTranslation( typename SpatialTransform<NT>::vector<3>
        { r_vector[0], r_vector[1], 0 }
    );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
SpatialTransform<NT>
SpatialTransform<NT>::makeRotation( const Quaternion<NT>& r_quaternion,
                                    const typename SpatialTransform<NT>::vector<3>& r_reference )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using ::operator*;
    auto transform = SpatialTransform<NT>::makeTranslation( -1 * r_reference );
    
    typename SpatialTransform<NT>::matrix_wrapper tmp;
    static_cast<typename SpatialTransform::matrix_type&>(tmp).setIdentity();

    r_quaternion.toRotationMatrix( tmp );
    transform.matrix() = static_cast<typename SpatialTransform::matrix_type&>(tmp) * transform.matrix();

    tmp = SpatialTransform<NT>::makeTranslation( r_reference )._transformationMatrix;
    transform.matrix() = static_cast<typename SpatialTransform::matrix_type&>(tmp) * transform.matrix();

    return transform;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
SpatialTransform<NT>
SpatialTransform<NT>::makeRotation( const typename SpatialTransform<NT>::vector<3>& r_axis,
                                    NT angle,
                                    const typename SpatialTransform<NT>::vector<3>& r_reference )
{
    CIE_BEGIN_EXCEPTION_TRACING

    Quaternion<NT> quaternion;
    quaternion.loadFromAxisAndAngle( r_axis, angle );

    return SpatialTransform<NT>::makeRotation( quaternion, r_reference );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
SpatialTransform<NT>
SpatialTransform<NT>::makeRotation( NT angle,
                                    const typename SpatialTransform<NT>::vector<2>& r_reference )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return SpatialTransform<NT>::makeRotation(
        typename SpatialTransform<NT>::vector<3> { 0, 0, 1 },
        angle, 
        typename SpatialTransform<NT>::vector<3> { r_reference[0], r_reference[1], 0.0 }
    );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
inline typename SpatialTransform<NT>::matrix_type&
SpatialTransform<NT>::matrix()
{
    return static_cast<typename SpatialTransform<NT>::matrix_type&>(this->_transformationMatrix);
}


template <concepts::NumericType NT>
inline const typename SpatialTransform<NT>::matrix_type&
SpatialTransform<NT>::matrix() const
{
    return static_cast<const typename SpatialTransform<NT>::matrix_type&>(this->_transformationMatrix);
}


template <concepts::NumericType NT>
inline const typename SpatialTransform<NT>::matrix_wrapper&
SpatialTransform<NT>::transformationMatrix() const
{
    return this->_transformationMatrix;
}


template <concepts::NumericType NT>
std::ostream&
operator<<( std::ostream& r_stream, const SpatialTransform<NT>& r_transform )
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( Size rowIndex=0; rowIndex<4; ++rowIndex )
    {
        for ( Size columnIndex=0; columnIndex<3; ++columnIndex )
            r_stream << r_transform.transformationMatrix()( rowIndex, columnIndex ) << ',';

        r_stream << r_transform.transformationMatrix()( rowIndex, 3 ) << std::endl;
    }

    return r_stream;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::linalg


#endif