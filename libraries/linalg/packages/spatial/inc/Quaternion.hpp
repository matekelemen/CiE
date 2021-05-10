#ifndef LINALG_QUATERNION_HPP
#define LINALG_QUATERNION_HPP

// ---- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "linalg/packages/types/inc/MatrixWrapper.hpp"

// --- STL Includes ---
#include <array>


namespace cie::linalg {


template <concepts::NumericType NT>
class Quaternion
{
public:
    using component_container = std::array<NT,4>;
    using iterator            = component_container::iterator;
    using const_iterator      = component_container::const_iterator;

public:
    Quaternion( component_container&& r_components );
    
    Quaternion( const component_container& r_components );

    Quaternion();

    Quaternion( Quaternion<NT>&& r_rhs ) = default;

    Quaternion( const Quaternion<NT>& r_rhs ) = default;

    Quaternion<NT>& operator=( const Quaternion<NT>& r_rhs ) = default;

public:
    void conjugate();

    void normalize();

    NT normSquared() const;

    template <class AxisType>
    requires concepts::ClassContainer<AxisType,NT>
    void loadFromAxisAndAngle( AxisType&& r_axis, NT angle );

    template <class MatrixType>
    void toRotationMatrix( MatrixWrapper<MatrixType>& r_matrix ) const;

    const component_container& components() const;

    NT operator[]( Size index ) const;
    NT& operator[]( Size index );

    const_iterator begin() const;
    iterator begin();

    const_iterator end() const;
    iterator end();

public:
    void operator*=( NT coefficient );

    void operator/=( NT denominator );

    void operator+=( const Quaternion<NT>& r_rhs);

    void operator-=( const Quaternion<NT>& r_rhs);
    
    void operator*=( const Quaternion<NT>& r_rhs);

protected:
    component_container  _components;
};


template <concepts::NumericType NT>
Quaternion<NT> operator+( const Quaternion<NT>& r_lhs, const Quaternion<NT>& r_rhs );

template <concepts::NumericType NT>
Quaternion<NT> operator-( const Quaternion<NT>& r_lhs, const Quaternion<NT>& r_rhs );

template <concepts::NumericType NT>
Quaternion<NT> operator*( const Quaternion<NT>& r_lhs, const Quaternion<NT>& r_rhs );


} // namespace cie::linalg

#include "linalg/packages/spatial/impl/Quaternion_impl.hpp"

#endif