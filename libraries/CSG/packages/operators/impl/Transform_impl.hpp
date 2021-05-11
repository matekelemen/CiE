#ifndef CIE_CSG_TRANSFORM_IMPL_HPP
#define CIE_CSG_TRANSFORM_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"


namespace cie::csg {


template < Size D,
           concepts::CopyConstructible VT,
           concepts::NumericType CT >
requires (D==2 || D==3)
template <class ...Args>
Transform<D,VT,CT>::Transform( typename Transform<D,VT,CT>::operand_ptr p_operand, Args&&... r_args ) :
    UnaryOperator<D,VT,CT>( p_operand ),
    linalg::SpatialTransform<CT>( std::forward<Args>(r_args)... )
{
}


template < Size D,
           concepts::CopyConstructible VT,
           concepts::NumericType CT >
requires (D==2 || D==3)
Transform<D,VT,CT>&
Transform<D,VT,CT>::set( typename Transform<D,VT,CT>::transform_type&& r_transform )
{
    CIE_BEGIN_EXCEPTION_TRACING

    static_cast<linalg::SpatialTransform<CT>&>( *this ) = r_transform;
    return *this;

    CIE_END_EXCEPTION_TRACING
}


template < Size D,
           concepts::CopyConstructible VT,
           concepts::NumericType CT >
requires (D==2 || D==3)
inline VT
Transform<D,VT,CT>::at( const typename Transform<D,VT,CT>::point_type& r_point ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( this->_p_rhs )

    return this->_p_rhs->at( this->transform(r_point) );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg


#endif