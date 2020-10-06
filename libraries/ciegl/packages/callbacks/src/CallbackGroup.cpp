// --- Internal Includes ---
#include "ciegl/packages/callbacks/inc/CallbackGroup.hpp"


namespace cie::gl {


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
        glm::dvec3 dx            = camera->mousePressPosition()
                                    - currentPosition
                                    - glm::dot( camera->direction(), camera->mousePressPosition() - currentPosition )
                                        / glm::dot( camera->direction(), currentPosition - camera->position() )
                                        * ( currentPosition - camera->position() );
        camera->translate( dx );
    }

    // ADVANCE
    else if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS )
    {
        glm::dvec3 normalProjection  = camera->screenToWorld( w/2.0, h/2.0 ) - camera->position();
        double multiplier          = 0.01;
        if ((double)y - camera->cursorPosition()[1] < 0.0)
            multiplier *= -1.0;

        camera->translate( multiplier * glm::length(normalProjection) * camera->direction() );
        
        camera->setCursorPosition( glm::dvec3((double)x, (double)y, 0.0) );
    }

    // ROTATE
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
    {
        double radius  = glm::length( camera->position() - camera->mousePressPosition() );
        
        // Project cursor onto the sphere defined by the click position (and the camera position as center)
        auto currentPosition    = camera->screenToWorld(x, y);
        currentPosition         = camera->position()
                                    + glm::normalize(currentPosition - camera->position()) 
                                        * radius;

        glm::dvec3 axis  = glm::cross( currentPosition-camera->position(), camera->mousePressPosition()-camera->position() );
        double angle   = -2.0*glm::asin( glm::length(currentPosition-camera->cursorPosition())/2.0 / radius );

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
    if (action == GLFW_PRESS)
    {
        if ( button==GLFW_MOUSE_BUTTON_1 || button==GLFW_MOUSE_BUTTON_2 )
        {
            camera->setMousePressPosition( glm::dvec3( (double)x, (double)y, 0.0) );
            camera->setCursorPosition( glm::dvec3( (double)x, (double)y, 0.0) );
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
        double dx       = ( x - camera->cursorPosition()[0] ) / w;
        double dy       = ( y - camera->cursorPosition()[1] ) / h;
        double angle    = -2.0 * glm::pi<double>() * glm::sqrt(dx*dx + dy*dy);

        glm::dvec3 axis  =   dy/glm::abs(dx+dy) * glm::cross( camera->up(), camera->direction() )
                            + dx/glm::abs(dx+dy) * camera->up();

        //std::cout << camera->up()[0] << "\t" << camera->up()[1] << "\t" << camera->up()[2] << "\n";
        //std::cout << camera->direction()[0] << "\t" << camera->direction()[1] << "\t" << camera->direction()[2] << "\n\n";
        camera->rotate( angle, axis );
        camera->setCursorPosition( glm::dvec3(x, y, 0.0) );
    }

    // Advance
    else if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS )
    {
        double dy       = ( y - camera->cursorPosition()[1] ) / (double)h;
        camera->translate( dy * glm::length(camera->position()-camera->center()) * camera->direction() );
        camera->setCursorPosition( glm::dvec3(x, y, 0.0) );
    }
}


} // namespace cie::gl