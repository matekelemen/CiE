#ifndef CIE_UTILS_STL_EXTENSION_TYPE_SAFETY_HPP
#define CIE_UTILS_STL_EXTENSION_TYPE_SAFETY_HPP

// --- Internal Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"


namespace cie::utils {

template <concepts::Deriveable T, class TypeTag>
struct TypeDefBase : public T, public TypeTag
{
    template <class ...Args>
    TypeDefBase( Args&&... args ) :
        T( std::forward<Args>(args)... )
    {}
}; // struct TypeDefBase (non-integral type)

} // namespace cie::utils


/**
 * TODO: this implementation doesn't work as intended
*/
#define STRONG_TYPEDEF( type_definition, type_name )                    \
    struct type_tag ## type_name {};                                    \
    typedef cie::utils::TypeDefBase<                                    \
        type_definition,                                                \
        type_tag ## type_name>    type_name;
#endif