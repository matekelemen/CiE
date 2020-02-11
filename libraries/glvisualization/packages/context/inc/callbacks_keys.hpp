#ifndef GLVISUALIZATION_CALLBACKS_KEYS_HPP
#define GLVISUALIZATION_CALLBACKS_KEYS_HPP

#include <GLFW/glfw3.h>
#include <functional>

namespace cie {
namespace gl {


using KeyCallbackFunction = GLFWkeyfun;


void callback_keyExit(      GLFWwindow* window, 
                            int key, 
                            int scancode, 
                            int action, 
                            int mods );


}
}

#endif