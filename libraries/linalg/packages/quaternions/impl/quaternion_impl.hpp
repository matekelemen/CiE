#ifndef CIE_LINALG_QUATERNION_IMPL_HPP
#define CIE_LINALG_QUATERNION_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "linalg/packages/overloads/inc/vectoroperators.hpp"


namespace cie::linalg {


template <concepts::NumericType NT>
Quaternion<NT>::Quaternion( typename Quaternion<NT>::component_container&& r_components ) :
    _components( std::move(r_components) )
{
}


template <concepts::NumericType NT>
Quaternion<NT>::Quaternion( const typename Quaternion<NT>::component_container& r_components ) :
    _components( r_components )
{
}


template <concepts::NumericType NT>
Quaternion<NT>::Quaternion() :
    _components( {1, 0, 0, 0} )
{
}


template <concepts::NumericType NT>
inline void
Quaternion<NT>::operator+=( const Quaternion<NT>& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using ::operator+=;
    this->_components += r_rhs._components;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
inline void
Quaternion<NT>::operator-=( const Quaternion<NT>& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using ::operator-=;
    this->_components -= r_rhs._components;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
void
Quaternion<NT>::operator*=( const Quaternion<NT>& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto tmp = this->_components;

    this->_components[0] = tmp[0]*r_rhs[0] - tmp[1]*r_rhs[1] - tmp[2]*r_rhs[2] - tmp[3]*r_rhs[3];
    this->_components[1] = tmp[0]*r_rhs[1] + tmp[1]*r_rhs[0] + tmp[2]*r_rhs[3] - tmp[3]*r_rhs[2];
    this->_components[2] = tmp[0]*r_rhs[2] + tmp[2]*r_rhs[0] + tmp[3]*r_rhs[1] - tmp[1]*r_rhs[3];
    this->_components[3] = tmp[0]*r_rhs[3] + tmp[3]*r_rhs[0] + tmp[1]*r_rhs[2] - tmp[2]*r_rhs[1];

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
inline void
Quaternion<NT>::conjugate()
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it_component = this->_components.begin() + 1;
    *it_component++ *= -1;
    *it_component++ *= -1;
    *it_component *= -1;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
void
Quaternion<NT>::normalize()
{
    CIE_BEGIN_EXCEPTION_TRACING

    NT magnitude = std::sqrt( this->normSquared() );
    if ( magnitude == 0 )
        CIE_THROW( DivisionByZeroException, "Cannot normalize a null-quaternion" )

    auto it_component = this->_components.begin();
    *it_component++ /= magnitude;
    *it_component++ /= magnitude;
    *it_component++ /= magnitude;
    *it_component /= magnitude;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
inline NT
Quaternion<NT>::normSquared() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it_component = this->_components.begin();
    NT output = (*it_component) * (*it_component++);
    output += (*it_component) * (*it_component++);
    output += (*it_component) * (*it_component);

    return output;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
inline const typename Quaternion<NT>::component_container&
Quaternion<NT>::components() const
{
    return this->_components;
}


template <concepts::NumericType NT>
inline NT
Quaternion<NT>::operator[]( Size index ) const
{
    CIE_OUT_OF_RANGE_CHECK( index < this->_components.size() )
    return _components[index];
}


template <concepts::NumericType NT>
inline NT&
Quaternion<NT>::operator[]( Size index )
{
    CIE_OUT_OF_RANGE_CHECK( index < this->_components.size() )
    return _components[index];
}


template <concepts::NumericType NT>
inline typename Quaternion<NT>::const_iterator
Quaternion<NT>::begin() const
{
    return _components.begin();
}


template <concepts::NumericType NT>
inline typename Quaternion<NT>::iterator
Quaternion<NT>::begin()
{
    return _components.begin();
}


template <concepts::NumericType NT>
inline typename Quaternion<NT>::const_iterator
Quaternion<NT>::end() const
{
    return _components.end();
}


template <concepts::NumericType NT>
inline typename Quaternion<NT>::iterator
Quaternion<NT>::end()
{
    return _components.end();
}


template <concepts::NumericType NT>
Quaternion<NT> operator+( const Quaternion<NT>& r_lhs, const Quaternion<NT>& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    Quaternion<NT> output( r_lhs );
    output += r_rhs;
    return output;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
Quaternion<NT> operator-( const Quaternion<NT>& r_lhs, const Quaternion<NT>& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    Quaternion<NT> output( r_lhs );
    output -= r_rhs;
    return output;

    CIE_END_EXCEPTION_TRACING
}

template <concepts::NumericType NT>
Quaternion<NT> operator*( const Quaternion<NT>& r_lhs, const Quaternion<NT>& r_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    Quaternion<NT> output( r_lhs );
    output *= r_rhs;
    return output;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::linalg

#endif