#ifndef GLVISUALIZATION_CALLBACKS_KEYS_HPP
#define GLVISUALIZATION_CALLBACKS_KEYS_HPP

#include <GLFW/glfw3.h>

namespace cie {
namespace gl {


void callback_keyExit(      GLFWwindow* window, 
                            int key, 
                            int scancode, 
                            int action, 
                            int mods );


}
}

#endif