#ifndef CIEGL_CALLBACKS_KEYS_HPP
#define CIEGL_CALLBACKS_KEYS_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- STL Includes ---
#include <functional>

namespace cie::gl {


using KeyCallbackFunction       = GLFWkeyfun;
using CursorCallbackFunction    = GLFWcursorposfun;
using MouseCallbackFunction     = GLFWmousebuttonfun;


void callback_keyExit(  GLFWwindow* window, 
                        int key, 
                        int scancode, 
                        int action, 
                        int mods );

void defaultCursorCallbackFunction( GLFWwindow* window,
                                    double x,
                                    double y);

void defaultMouseCallbackFunction(  GLFWwindow* window,
                                    int button,
                                    int action,
                                    int mods);

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


} // namespace cie::gl

#endif