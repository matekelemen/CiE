// --- Internal Includes ---
#include "ciegl/packages/callbacks/inc/CallbackGroup.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"


namespace cie::gl {


void CallbackGroup::keyCallback(    WindowPtr p_window,
                                    int key,
                                    int scanCode,
                                    int action,
                                    int mods    )
{
    auto p_rawWindow         = detail::getGLFWwindow( p_window );
    DrawManager* drawManager = static_cast<DrawManager*>(glfwGetWindowUserPointer( p_rawWindow ));

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
                    glfwSetWindowShouldClose( p_rawWindow, GL_TRUE );
                    break;
                default:
                    break;
            }
}


void CallbackGroup::mouseCallback(  WindowPtr p_window,
                                    int button,
                                    int action,
                                    int mods    )
{
    // Get cursor position
    auto cursorPosition = p_window->getCursorPosition();
    //double x, y;
    //glfwGetCursorPos( window, &x, &y );

    // Flip y
    const auto& windowSize = p_window->getSize();
    cursorPosition.second = double(windowSize.second) - cursorPosition.second;

    // Get DrawManager and InteractiveCamera
    auto camera =  getCameraPtr<InteractiveCamera>( p_window );

    // Set press position if pressed
    if (action == GLFW_PRESS && action != GLFW_REPEAT)
    {
        if ( button==GLFW_MOUSE_BUTTON_1 || button==GLFW_MOUSE_BUTTON_2 || button==GLFW_MOUSE_BUTTON_3 )
        {
            auto worldPos = camera->screenToWorld( cursorPosition.first, cursorPosition.second );
            camera->setMousePressPosition( worldPos );
            camera->setCursorPosition( worldPos );
        }
    }
}


void CallbackGroup::cursorCallback( WindowPtr p_window,
                                    double x,
                                    double y   )
{
    // Flip y
    const auto& windowSize = p_window->getSize();
    y = double(windowSize.second) - y;

    // Cast pointers
    auto p_rawWindow = detail::getGLFWwindow( p_window );
    auto p_camera    =  getCameraPtr<InteractiveCamera>( p_window );

    // STRAFE
    if ( glfwGetMouseButton( p_rawWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS )
    {
        // Project the picked point onto the plane defined by
        // the camera direction as normal, and the previous point,
        // then compute the difference of the projection and the previous point
        auto currentPosition    = p_camera->screenToWorld(x, y);
        glm::vec3 dx            = p_camera->mousePressPosition()
                                    - currentPosition
                                    - glm::dot( p_camera->direction(), p_camera->mousePressPosition() - currentPosition )
                                        / glm::dot( p_camera->direction(), currentPosition - p_camera->position() )
                                        * ( currentPosition - p_camera->position() );
        p_camera->translate( dx );
    }

    // ADVANCE
    else if ( glfwGetMouseButton( p_rawWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS )
    {
        glm::dvec3 normalProjection  = p_camera->screenToWorld( w/2.0, h/2.0 ) - p_camera->position();
        double multiplier          = 0.01;
        if ((double)y - p_camera->cursorPosition()[1] < 0.0)
            multiplier *= -1.0;

        p_camera->translate( multiplier * glm::length(normalProjection) * p_camera->direction() );
        
        p_camera->setCursorPosition( glm::dvec3((double)x, (double)y, 0.0) );
    }

    // ROTATE
    else if (glfwGetMouseButton( p_rawWindow, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
    {
        double radius  = glm::length( p_camera->position() - p_camera->mousePressPosition() );
        
        // Project cursor onto the sphere defined by the click position (and the camera position as center)
        auto currentPosition    = p_camera->screenToWorld(x, y);
        currentPosition         = p_camera->position()
                                    + glm::normalize(currentPosition - p_camera->position()) 
                                        * radius;

        glm::dvec3 axis  = glm::cross( currentPosition-p_camera->position(), p_camera->mousePressPosition()-p_camera->position() );
        double angle   = -2.0*glm::asin( glm::length(currentPosition-p_camera->cursorPosition())/2.0 / radius );

        p_camera->rotate( angle, 
                          axis,
                          p_camera->mousePressPosition() );

        p_camera->setCursorPosition( currentPosition );
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