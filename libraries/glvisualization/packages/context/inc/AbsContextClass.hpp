#ifndef GLVISUALIZATION_ABS_CONTEXT_CLASS_HPP
#define GLVISUALIZATION_ABS_CONTEXT_CLASS_HPP

// --- Internal Includes ---
#include "GLContext.hpp"

// --- STL Includes ---
#include <string>


namespace cie{
namespace gl {


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


}
}

#include "AbsContextClass_impl.hpp"

#endif