#ifndef GLVISUALIZATION_MOUSE_CALLBACKS_HPP
#define GLVISUALIZATION_MOUSE_CALLBACKS_HPP

#include "../../drawing/inc/DrawManager.hpp"
#include "../../drawing/inc/Camera.hpp"
#include "../../context/inc/defaultCallback.hpp"
#include <functional>

namespace cie {
namespace gl {

CursorCallbackFunction makeCursorCallbackFunction(  CursorCallbackFunction function, 
                                                    GLContext& context,
                                                    DrawManager& drawManager );

MouseCallbackFunction makeMouseCallbackFunction(    MouseCallbackFunction function,
                                                    GLContext& context,
                                                    DrawManager& drawManager);

void arcballMouseCallback(  GLFWwindow* window,
                            int button,
                            int action,
                            int mods);

}
}

#endif