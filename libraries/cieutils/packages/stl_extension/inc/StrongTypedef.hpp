#ifndef CIE_UTILS_STL_EXTENSION_STRONG_TYPEDEF_HPP
#define CIE_UTILS_STL_EXTENSION_STRONG_TYPEDEF_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"


namespace cie::utils {


/**
 * Wrapper class for creating non-interchangable typedefs
 * 
 * (Strong typedefd classes will be called subclasses of the wrapped base class)
 *  1) Subclasses must be constructible from- and converted to- the wrapped base class
 *  2) Subclasses derived from the same base class must not be implicitly convertible to each other
 *  3) Overloaded operators must be able to interoperate between sub- and base classes, as well as solely with subclasses
 */
template <class T, class Tag>
class AbsStrongTypedef
{
public:
    using base_type = T;

public:
    AbsStrongTypedef();
    AbsStrongTypedef( T base );
    AbsStrongTypedef( T& r_base );
    AbsStrongTypedef( const T& r_base );

    AbsStrongTypedef( const AbsStrongTypedef<T,Tag>& r_rhs );
    AbsStrongTypedef<T,Tag>& operator=( const AbsStrongTypedef<T,Tag>& r_rhs );

    ~AbsStrongTypedef();

    operator T() const;

    const T& get() const;

private:
    void throwIfUninitialized() const;

private:
    T* _p_base;
};


template <class T, class Tag>
class StrongTypedef : public AbsStrongTypedef<T,Tag>
{
    template <class ...Args>
    StrongTypedef( Args&&... args );
};


} // namespace cie::utils


#define STRONG_TYPEDEF( BaseType, SubType )                                                 \
    struct _typedef_ ## SubType ## _ ## BaseType ## _tag {};                                \
    typedef cie::utils::AbsStrongTypedef<                                                   \
                            BaseType ,                                                      \
                           _typedef_  ## SubType ## _ ## BaseType ## _tag> SubType;



#include "cieutils/packages/stl_extension/impl/StrongTypedef_impl.hpp"
#include "cieutils/packages/stl_extension/inc/StrongTypedef_overloads.hpp"

#endif