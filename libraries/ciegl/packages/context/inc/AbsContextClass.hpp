#ifndef CIEGL_ABS_CONTEXT_CLASS_HPP
#define CIEGL_ABS_CONTEXT_CLASS_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/NamedObject.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWContext.hpp"

// --- STL Includes ---
#include <string>


namespace cie::gl {


class AbsContextClass : public utils::Loggee
{
public:
    const GLFWContext& context() const;

protected:
    AbsContextClass(    GLFWContext& r_context,
                        const std::string& r_name );
    void checkGLErrors( const std::string& r_errorMessage );

    GLFWContext&    _r_context;
};


} // namespace cie::gl

#include "ciegl/packages/context/impl/AbsContextClass_impl.hpp"

#endif