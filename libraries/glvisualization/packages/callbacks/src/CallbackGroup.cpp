// --- Internal Includes ---
#include "../inc/CallbackGroup.hpp"


namespace cie {
namespace gl {


void CallbackGroup::keyCallback(    WindowPtr window,
                                    int key,
                                    int scanCode,
                                    int action,
                                    int mods    )
{
    DrawManager* drawManager = static_cast<DrawManager*>(glfwGetWindowUserPointer( window ));

    if ( action == GLFW_PRESS || action==GLFW_REPEAT )
            switch (key)
            {
                case GLFW_KEY_KP_ADD:
                    drawManager->camera()->zoom( 0.25 );
                    break;
                case GLFW_KEY_KP_SUBTRACT:
                    drawManager->camera()->zoom( -0.25 );
                    break;
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose( window, GL_TRUE );
                    break;
                default:
                    break;
            }
}


void CallbackGroup::mouseCallback(  WindowPtr window,
                                    int button,
                                    int action,
                                    int mods    )
{
    // Get cursor position
    double x, y;
    glfwGetCursorPos( window, &x, &y );

    // Get DrawManager and InteractiveCamera
    DrawManager* drawManager    = static_cast<DrawManager*>(glfwGetWindowUserPointer( window ));
    InteractiveCameraPtr camera     = std::static_pointer_cast<InteractiveCamera>(drawManager->camera());

    // Set press position if pressed
    if (action == GLFW_PRESS)
    {
        if ( button==GLFW_MOUSE_BUTTON_1 || button==GLFW_MOUSE_BUTTON_2 )
        {
            camera->setMousePressPosition( x, y );
        }
    }
}


void CallbackGroup::cursorCallback( WindowPtr window,
                                    double x,
                                    double y   )
{
    DrawManager* manager = static_cast<DrawManager*>(glfwGetWindowUserPointer(window));
    InteractiveCamera* camera = static_cast<InteractiveCamera*>( manager->camera().get() );

    // STRAFE
    if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS )
    {
        auto currentPosition    = camera->screenToWorld( x, y );
        camera->translate( camera->mousePressPosition() - currentPosition );
        camera->setMousePressPosition( currentPosition );
    }

    // ADVANCE
    if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS )
    {
        glm::vec3 currentPosition   = camera->screenToWorld( x, y );
        glm::vec3 dx                = camera->mousePressPosition() - currentPosition;
        GLfloat dz                  = glm::abs(dx[0])*dx[0] + glm::abs(dx[1])*dx[1];
        camera->setMousePressPosition( currentPosition );

        camera->translate(  glm::vec3(  0.0, 
                                        0.0, 
                                        glm::sign(dz) * glm::sqrt(glm::abs(dz)) ) 
                            );
    }
}


}
}