#ifndef GLVISUALIZATION_CALLBACKS_KEYS_HPP
#define GLVISUALIZATION_CALLBACKS_KEYS_HPP

#include <GLFW/glfw3.h>
#include <functional>
#include "../../logging/inc/GLLogger.hpp"

namespace cie {
namespace gl {


using KeyCallbackFunction = GLFWkeyfun;


void callback_keyExit(      GLFWwindow* window, 
                            int key, 
                            int scancode, 
                            int action, 
                            int mods );

void GLAPIENTRY messageCallback(    GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar* message,
                                    const void* logger );
                            
void callback_errorPrint(int error, const char* description);

void frameBufferResizeCallback( GLFWwindow* window,
                                int width,
                                int height  );


}
}

#endif