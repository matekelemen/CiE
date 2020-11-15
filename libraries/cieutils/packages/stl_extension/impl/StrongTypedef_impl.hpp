#ifndef CIE_UTILS_STL_EXTENSION_STRONG_TYPEDEF_IMPL_HPP
#define CIE_UTILS_STL_EXTENSION_STRONG_TYPEDEF_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::utils {


template <class T, class Tag>
AbsStrongTypedef<T,Tag>::AbsStrongTypedef() :
    _p_base( nullptr )
{
}


template <class T, class Tag>
AbsStrongTypedef<T,Tag>::AbsStrongTypedef( T base ) :
    _p_base( new T( base ) )
{
}


template <class T, class Tag>
AbsStrongTypedef<T,Tag>::AbsStrongTypedef( T& r_base ) :
    _p_base( new T( r_base ) )
{
    this->_p_base = new T( r_base );
}


template <class T, class Tag>
AbsStrongTypedef<T,Tag>::AbsStrongTypedef( const T& r_base ) :
    _p_base( new T( r_base ) )
{
    this->_p_base = new T( r_base );
}


template <class T, class Tag>
AbsStrongTypedef<T,Tag>::AbsStrongTypedef( const AbsStrongTypedef<T,Tag>& r_rhs )
{
    if ( r_rhs._p_base )
        this->_p_base = new T( *r_rhs._p_base );
    else
        this->_p_base = nullptr;
}


template <class T, class Tag>
AbsStrongTypedef<T,Tag>&
AbsStrongTypedef<T,Tag>::operator=( const AbsStrongTypedef<T,Tag>& r_rhs )
{
    if ( r_rhs._p_base )
    {
        if ( this->_p_base )
            *this->_p_base = *r_rhs._p_base;
        else
            this->_p_base = new T( *r_rhs._p_base );
    }
    else
        this->_p_base = nullptr;
}


template <class T, class Tag>
AbsStrongTypedef<T,Tag>::~AbsStrongTypedef()
{
    if ( this->_p_base )
        delete this->_p_base;
}


template <class T, class Tag>
AbsStrongTypedef<T,Tag>::operator T() const
{
    return this->get();
}


template <class T, class Tag>
inline const T&
AbsStrongTypedef<T,Tag>::get() const
{
    this->throwIfUninitialized();
    return *this->_p_base;
}


template <class T, class Tag>
inline void
AbsStrongTypedef<T,Tag>::throwIfUninitialized() const
{
    if ( !this->_p_base )
        CIE_THROW( NullPtrException, "Uninitialized strong typedef!" )
}


template <class T, class Tag>
template <class ...Args>
StrongTypedef<T,Tag>::StrongTypedef( Args&&... args ) :
    AbsStrongTypedef<T,Tag>( std::forward<Args>(args)... )
{
}


} // namespace cie::utils


#endif