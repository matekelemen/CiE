#ifndef GLVISUALIZATION_ABS_CONTEXT_CLASS_IMPL_HPP
#define GLVISUALIZATION_ABS_CONTEXT_CLASS_IMPL_HPP

namespace cie {
namespace gl {


template <class T>
void AbsContextClass::logID(  const std::string& message,
                            T id,
                            GLuint messageType )
{
    log( message + " | ID_" + std::to_string(id), messageType );
}


}
}

#endif