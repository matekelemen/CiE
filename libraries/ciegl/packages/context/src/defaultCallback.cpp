// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWContext.hpp"

// --- STL Includes ---
#include <iostream>
#include <string>

namespace cie::gl {


void callback_keyExit(  GLFWwindow* window, 
                        int key, 
                        int scancode, 
                        int action, 
                        int mods )
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


void defaultCursorCallbackFunction( GLFWwindow* window,
                                    double x,
                                    double y)
{
}


void defaultMouseCallbackFunction(  GLFWwindow* window,
                                    int button,
                                    int action,
                                    int mods)
{
    if (button==GLFW_MOUSE_BUTTON_1 && action==GLFW_PRESS && mods==GLFW_MOD_SHIFT)
    {
        double x, y;
        glfwGetCursorPos( window, &x, &y );
        std::cout << x << ", " << y << "\n";
    }
}


void GLAPIENTRY messageCallback(    GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar* message,
                                    const void* logger )
{
    std::string messageString = message;
    auto p_context = const_cast<GLFWContext*>( reinterpret_cast<const GLFWContext*>( logger ) );
    p_context->log( std::to_string(source) + ": " + message + "\n" );
}


void callback_errorPrint(int error, const char* description)
{
    std::cout << description << " (error code " + std::to_string( error ) + ")\n";
}


void frameBufferResizeCallback( GLFWwindow* window,
                                int width,
                                int height  )
{
    std::cout << "Frame buffer resize!\n";
    glViewport( 0, 0, width, height );
}


} // namespace cie::gl