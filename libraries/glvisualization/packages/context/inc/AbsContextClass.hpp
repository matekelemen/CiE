#ifndef GLVISUALIZATION_ABS_CONTEXT_CLASS_HPP
#define GLVISUALIZATION_ABS_CONTEXT_CLASS_HPP

#include "GLContext.hpp"
#include <string>

namespace cie{
namespace gl {


class AbsContextClass
{
protected:
    void log(   const std::string& message,
                GLuint messageType = CONTEXT_LOG_TYPE_REPORT );
    
    template <class T>
    void logID( const std::string& message,
                T id,
                GLuint messageType = CONTEXT_LOG_TYPE_REPORT );

    AbsContextClass(    GLContext& context,
                        const std::string& className );

    void terminate();



    std::string     _className;
    GLContext*      _context;
};


}
}

#include "AbsContextClass_impl.hpp"

#endif