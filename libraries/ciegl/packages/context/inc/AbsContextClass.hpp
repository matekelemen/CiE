#ifndef CIEGL_ABS_CONTEXT_CLASS_HPP
#define CIEGL_ABS_CONTEXT_CLASS_HPP

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLContext.hpp"

// --- STL Includes ---
#include <string>


namespace cie::gl {


class AbsContextClass : public utils::Loggee
{
public:
    virtual void terminate();
    const GLContext* context() const;

protected:
    AbsContextClass(    GLContext& context,
                        const std::string& className );
    void checkGLErrors( const std::string& errorMessage = "" );

    std::string     _className;
    GLContext*      _context;
};


} // namespace cie::gl

#include "ciegl/packages/context/impl/AbsContextClass_impl.hpp"

#endif