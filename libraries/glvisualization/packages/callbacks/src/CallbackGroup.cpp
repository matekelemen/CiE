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

    // Flip y
    int w, h;
    glfwGetFramebufferSize( window, &w, &h );
    y = (double)h - y;

    // Get DrawManager and InteractiveCamera
    auto camera =  getCameraPtr<InteractiveCamera>( window );

    // Set press position if pressed
    if (action == GLFW_PRESS && action != GLFW_REPEAT)
    {
        if ( button==GLFW_MOUSE_BUTTON_1 || button==GLFW_MOUSE_BUTTON_2 || button==GLFW_MOUSE_BUTTON_3 )
        {
            auto worldPos = camera->screenToWorld(x, y);
            camera->setMousePressPosition( worldPos );
            camera->setCursorPosition( worldPos );
        }
    }
}


void CallbackGroup::cursorCallback( WindowPtr window,
                                    double x,
                                    double y   )
{
    // Flip y
    int w, h;
    glfwGetFramebufferSize( window, &w, &h );
    y = (double)h - y;

    // Cast pointers
    auto camera =  getCameraPtr<InteractiveCamera>( window );

    // STRAFE
    if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS )
    {
        // Project the picked point onto the plane defined by
        // the camera direction as normal, and the previous point,
        // then compute the difference of the projection and the previous point
        auto currentPosition    = camera->screenToWorld(x, y);
        glm::vec3 dx            = camera->mousePressPosition()
                                    - currentPosition
                                    - glm::dot( camera->cameraDirection(), camera->mousePressPosition() - currentPosition )
                                        / glm::dot( camera->cameraDirection(), currentPosition - camera->cameraPosition() )
                                        * ( currentPosition - camera->cameraPosition() );
        camera->translate( dx );
    }

    // ADVANCE
    else if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS )
    {
        glm::vec3 normalProjection  = camera->screenToWorld( w/2.0, h/2.0 ) - camera->cameraPosition();
        GLfloat multiplier          = 0.01;
        if ((GLfloat)y - camera->cursorPosition()[1] < 0.0f)
            multiplier *= -1.0f;

        camera->translate( multiplier * glm::length(normalProjection) * camera->cameraDirection() );
        
        camera->setCursorPosition( glm::vec3((GLfloat)x, (GLfloat)y, 0.0f) );
    }

    // ROTATE
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
    {
        GLfloat radius  = glm::length( camera->cameraPosition() - camera->mousePressPosition() );
        
        // Project cursor onto the sphere defined by the click position (and the camera position as center)
        auto currentPosition    = camera->screenToWorld(x, y);
        currentPosition         = camera->cameraPosition()
                                    + glm::normalize(currentPosition - camera->cameraPosition()) 
                                        * radius;

        glm::vec3 axis  = glm::cross( currentPosition-camera->cameraPosition(), camera->mousePressPosition()-camera->cameraPosition() );
        GLfloat angle   = -2.0*glm::asin( glm::length(currentPosition-camera->cursorPosition())/2.0 / radius );

        camera->rotate( angle, 
                        axis,
                        camera->mousePressPosition() );

        camera->setCursorPosition( currentPosition );
    }
}


void ArcballCallbacks::mouseCallback(   WindowPtr window,
                                        int button,
                                        int action,
                                        int mods    )
{
    // Get cursor position
    double x, y;
    glfwGetCursorPos( window, &x, &y );

    // Flip y
    int w, h;
    glfwGetFramebufferSize( window, &w, &h );
    y = (double)h - y;

    // Get DrawManager and InteractiveCamera
    auto camera =  getCameraPtr<ArcballCamera>( window );

    // Set press position if pressed
    if (action == GLFW_PRESS && action != GLFW_REPEAT)
    {
        if ( button==GLFW_MOUSE_BUTTON_1 )
        {
            camera->setMousePressPosition( glm::vec3( (GLfloat)x, (GLfloat)y, 0.0f) );
            camera->setCursorPosition( glm::vec3( (GLfloat)x, (GLfloat)y, 0.0f) );
        }
    }
}



void ArcballCallbacks::cursorCallback(  WindowPtr window,
                                        double x,
                                        double y   )
{
    // Flip y
    int w, h;
    glfwGetFramebufferSize( window, &w, &h );
    y = (double)h - y;

    // Cast pointers
    auto camera =  getCameraPtr<ArcballCamera>( window );

    // Rotate
    if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS )
    {
        GLfloat dx      = ( x - camera->cursorPosition()[0] ) / w;
        GLfloat dy      = ( y - camera->cursorPosition()[1] ) / h;
        GLfloat angle   = -2.0 * glm::pi<GLfloat>() * glm::sqrt(dx*dx + dy*dy);

        glm::vec3 axis  =   dy/glm::abs(dx+dy) * glm::cross( camera->cameraUp(), camera->cameraDirection() )
                            + dx/glm::abs(dx+dy) * camera->cameraUp();

        std::cout << camera->cameraUp()[0] << "\t" << camera->cameraUp()[1] << "\t" << camera->cameraUp()[2] << "\n";
        std::cout << camera->cameraDirection()[0] << "\t" << camera->cameraDirection()[1] << "\t" << camera->cameraDirection()[2] << "\n\n";
        camera->rotate( angle, axis );
        camera->setCursorPosition( glm::vec3(x, y, 0.0f) );
    }
}


}
}