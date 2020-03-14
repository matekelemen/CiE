// --- Internal Includes ---
#include "../inc/key_callbacks.hpp"

namespace cie {
namespace gl {


KeyCallbackFunction makeKeyCallbackFunction(    KeyCallbackFunction function, 
                                                GLContext& context,
                                                DrawManager& drawManager )
{
    if (glfwGetWindowUserPointer(context.window()) == nullptr)
        glfwSetWindowUserPointer( context.window(), &drawManager );
    return function;
}


void arcballKeyCallback(    WindowPtr window,
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
                    zoomCamera( drawManager->camera(), true );
                    break;
                case GLFW_KEY_KP_SUBTRACT:
                    zoomCamera( drawManager->camera(), false );
                    break;
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose( window, GL_TRUE );
                    break;
                default:
                    break;
            }
}


void zoomCamera( CameraPtr camera, bool zoom )
{
    GLfloat modifier = 0.25f;

    if (zoom)
        modifier *= -1.0f;

    if ( camera->height()>-modifier && camera->width()>-modifier )
    {
        camera->height() += modifier * camera->height()/camera->width();
        camera->width()  += modifier;
    }

    if ( camera->fieldOfView() > -10*modifier )
        camera->setCameraProperties( camera->fieldOfView() + 10*modifier,
                                    camera->nearClippingPlane(),
                                    camera->farClippingPlane()   );

    camera->updateTransformationMatrix();
}


}
}