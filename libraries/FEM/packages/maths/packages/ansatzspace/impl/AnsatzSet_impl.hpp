#ifndef CIE_FEM_MATHS_ANSATZ_SET_IMPL_HPP
#define CIE_FEM_MATHS_ANSATZ_SET_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
AnsatzSet<Dimension,NT>::AnsatzSet( typename AnsatzSet<Dimension,NT>::ansatz_container&& r_ansatzFunctions ) :
    _ansatzFunctions( std::move(r_ansatzFunctions) )
{
}


template <Size Dimension, concepts::NumericType NT>
AnsatzSet<Dimension,NT>::AnsatzSet( const typename AnsatzSet<Dimension,NT>::ansatz_container& r_ansatzFunctions ) :
    _ansatzFunctions( r_ansatzFunctions )
{
}


template <Size Dimension, concepts::NumericType NT>
inline Size
AnsatzSet<Dimension,NT>::size() const
{
    return this->_ansatzFunctions.size();
}


template <Size Dimension, concepts::NumericType NT>
inline const typename AnsatzSet<Dimension,NT>::ansatz_function&
AnsatzSet<Dimension,NT>::operator[]( Size index ) const
{
    CIE_OUT_OF_RANGE_CHECK( index < this->_ansatzFunctions.size() )
    CIE_CHECK_POINTER( this->_ansatzFunctions[index] )

    return *this->_ansatzFunctions[index];
}


template <Size Dimension, concepts::NumericType NT>
inline const typename AnsatzSet<Dimension,NT>::ansatz_function&
AnsatzSet<Dimension,NT>::at( Size index ) const
{
    CIE_OUT_OF_RANGE_CHECK( index < this->_ansatzFunctions.size() )
    CIE_CHECK_POINTER( this->_ansatzFunctions[index] )

    return *this->_ansatzFunctions[index];
}


template <Size Dimension, concepts::NumericType NT>
inline void
AnsatzSet<Dimension,NT>::push_back( const typename AnsatzSet<Dimension,NT>::ansatz_ptr& rp_ansatzFunction )
{
    this->_ansatzFunctions.push_back( rp_ansatzFunction );
}


template <Size Dimension, concepts::NumericType NT>
template <class AnsatzFunction, class ...Args>
requires concepts::DerivedFrom<AnsatzFunction, typename AnsatzSet<Dimension,NT>::ansatz_function>
inline void
AnsatzSet<Dimension,NT>::emplace_back( Args&&... r_args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_ansatzFunction = std::make_shared<AnsatzFunction>( std::forward<Args>(r_args)... );

    this->_ansatzFunctions.push_back( p_ansatzFunction );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif