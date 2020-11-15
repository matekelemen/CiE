#ifndef CIE_UTILS_STRONG_TYPEDEF_OVERLOADS_IMPL_HPP
#define CIE_UTILS_STRONG_TYPEDEF_OVERLOADS_IMPL_HPP

namespace cie::utils {


/* --- LOGICAL OPERATORS --- */

template <class T, class Tag>
inline bool
operator==( const AbsStrongTypedef<T,Tag>& r_lhs,
            const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs.get() == r_rhs.get();
}

template <class T, class Tag>
inline bool
operator==( const AbsStrongTypedef<T,Tag>& r_lhs,
            const T& r_rhs )
{
    return r_lhs.get() == r_rhs;
}

template <class T, class Tag>
inline bool
operator==( const T& r_lhs,
                 const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs == r_rhs.get();
}


template <class T, class Tag>
inline bool
operator!=( const AbsStrongTypedef<T,Tag>& r_lhs,
            const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs.get() != r_rhs.get();
}

template <class T, class Tag>
inline bool
operator!=( const AbsStrongTypedef<T,Tag>& r_lhs,
            const T& r_rhs )
{
    return r_lhs.get() != r_rhs;
}

template <class T, class Tag>
inline bool
operator!=( const T& r_lhs,
            const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs == r_rhs.get();
}


template <class T, class Tag>
inline bool
operator<( const AbsStrongTypedef<T,Tag>& r_lhs,
           const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs.get() < r_rhs.get();
}

template <class T,class Tag>
inline bool
operator<( const AbsStrongTypedef<T,Tag>& r_lhs,
           const T& r_rhs )
{
    return r_lhs.get() < r_rhs;
}

template <class T, class Tag>
inline bool
operator<( const T& r_lhs,
           const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs < r_rhs.get();
}

template <class T, class Tag>
inline bool
operator>( const AbsStrongTypedef<T,Tag>& r_lhs,
           const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs.get() > r_rhs.get();
}

template <class T,class Tag>
inline bool
operator>( const AbsStrongTypedef<T,Tag>& r_lhs,
           const T& r_rhs )
{
    return r_lhs.get() > r_rhs;
}

template <class T, class Tag>
inline bool
operator>( const T& r_lhs,
           const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs > r_rhs.get();
}


template <class T, class Tag>
inline bool
operator<=( const AbsStrongTypedef<T,Tag>& r_lhs,
            const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs.get() <= r_rhs.get();
}

template <class T,class Tag>
inline bool
operator<=( const AbsStrongTypedef<T,Tag>& r_lhs,
            const T& r_rhs )
{
    return r_lhs.get() <= r_rhs;
}

template <class T, class Tag>
inline bool
operator<=( const T& r_lhs,
            const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs <= r_rhs.get();
}

template <class T, class Tag>
inline bool
operator>=( const AbsStrongTypedef<T,Tag>& r_lhs,
            const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs.get() >= r_rhs.get();
}

template <class T,class Tag>
inline bool
operator>=( const AbsStrongTypedef<T,Tag>& r_lhs,
            const T& r_rhs )
{
    return r_lhs.get() >= r_rhs;
}

template <class T, class Tag>
inline bool
operator>=( const T& r_lhs,
            const AbsStrongTypedef<T,Tag>& r_rhs )
{
    return r_lhs >= r_rhs.get();
}


/* --- ARITHMETIC OPERATORS --- */



/* --- STREAM OPERATORS --- */


} // namespace cie::utils

#endif