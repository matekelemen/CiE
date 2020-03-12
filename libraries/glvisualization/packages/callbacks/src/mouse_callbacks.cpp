#include "../inc/mouse_callbacks.hpp"

namespace cie {
namespace gl {


CursorCallbackFunction makeCursorCallbackFunction(  CursorCallbackFunction function,
                                                    GLContext& context,
                                                    DrawManager& drawManager)
{
    if (glfwGetWindowUserPointer(context.window()) == nullptr)
        glfwSetWindowUserPointer( context.window(), &drawManager );
    return function;
}


MouseCallbackFunction makeMouseCallbackFunction(    MouseCallbackFunction function,
                                                    GLContext& context,
                                                    DrawManager& drawManager)
{
    if (glfwGetWindowUserPointer(context.window()) == nullptr)
        glfwSetWindowUserPointer( context.window(), &drawManager );
    return function;
}


void arcballMouseCallback(  GLFWwindow* window,
                            int button,
                            int action,
                            int mods)
{
    // Get cursor position
    double x, y;
    glfwGetCursorPos( window, &x, &y );

    // Get DrawManager and ArcballCamera
    DrawManager* drawManager    = static_cast<DrawManager*>(glfwGetWindowUserPointer( window ));
    ArcballCameraPtr camera     = std::static_pointer_cast<ArcballCamera>(drawManager->camera());

    // Set press position if pressed
    if ( action==GLFW_PRESS )
    {
        std::cout << camera->mousePressPosition()[0] << ", " << camera->mousePressPosition()[1] << "\n";
        camera->setMousePressPosition( x, y );
    }
}


}
}