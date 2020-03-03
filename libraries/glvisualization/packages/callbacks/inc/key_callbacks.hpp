#ifndef GLVISUALIZATION_KEY_CALLBACKS_HPP
#define GLVISUALIZATION_KEY_CALLBACKS_HPP

#include "../../drawing/inc/DrawManager.hpp"
#include "../../drawing/inc/Camera.hpp"
#include <functional>

namespace cie {
namespace gl {

KeyCallbackFunction makeKeyCallbackFunction(    KeyCallbackFunction function, 
                                                GLContext& context,
                                                DrawManager& drawManager );

void defaultArcballKeyCallback( GLFWwindow* window,
                                int key,
                                int scanCode,
                                int action,
                                int mods    );

void zoomCamera( Camera& camera, bool zoom );
void closeWindow( DrawManager& drawManager );

}
}

#endif