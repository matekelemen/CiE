#include "../inc/defaultCallback.hpp"
#include <iostream>
#include <string>

namespace cie {
namespace gl {


void callback_keyExit(      GLFWwindow* window, 
                            int key, 
                            int scancode, 
                            int action, 
                            int mods )
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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
    std::cout <<  std::to_string(source) + ": " + message + "\n";
}


void callback_errorPrint(int error, const char* description)
{
    std::cout << description << " (error code " + std::to_string( error ) + ")\n";
}


}
}