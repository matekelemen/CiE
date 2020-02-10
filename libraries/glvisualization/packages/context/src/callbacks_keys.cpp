#include "../inc/callbacks_keys.hpp"

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


}
}