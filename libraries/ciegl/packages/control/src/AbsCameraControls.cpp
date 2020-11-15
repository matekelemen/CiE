// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/control/inc/AbsCameraControls.hpp"

// --- STL Includes ---
#include <functional>


namespace cie::gl {


AbsCameraControls::AbsCameraControls( WindowPtr p_window,
                                      CameraPtr p_camera ) :
    _p_window( p_window ),
    _p_camera( p_camera )
{
    this->bindToWindow( p_window );
}


void AbsCameraControls::mouseButtonCallback( KeyEnum button,
                                             KeyEnum action,
                                             KeyEnum modifiers )
{
    if ( action == GLFW_PRESS || action == GLFW_REPEAT )
        this->onMouseButtonPress( button, modifiers );
    else if ( action == GLFW_RELEASE )
        this->onMouseButtonRelease( button, modifiers );
    else
        _p_camera->log( "Unrecognized mouse button action: " + std::to_string(action),
                        LOG_TYPE_ERROR );
}


void AbsCameraControls::cursorPositionCallback( double x,
                                                double y )
{
    this->onCursorMovement( x, y );
}


void AbsCameraControls::cursorEnterCallback( KeyEnum entered )
{
    if ( entered == GLFW_TRUE )
        this->onCursorEnter();
    else if ( entered == GLFW_FALSE )
        this->onCursorLeave();
    else
        _p_camera->log( "Unrecognised cursor enter action: " + std::to_string(entered),
                        LOG_TYPE_ERROR );
}


void AbsCameraControls::scrollCallback( double xOffset,
                                        double yOffset )
{
    if ( xOffset != 0.0 )
        this->onHorizontalScroll( xOffset );
    if ( yOffset != 0.0 )
        this->onVerticalScroll( yOffset );
}


void AbsCameraControls::keyboardCallback( KeyEnum key,
                                          KeyEnum action,
                                          KeyEnum modifiers )
{
    if ( action==GLFW_PRESS || action==GLFW_REPEAT )
        this->onKeyboardPress( key, modifiers );
    else if ( action == GLFW_RELEASE )
        this->onKeyboardRelease( key, modifiers );
    else
        this->_p_camera->log( "Unrecognised keyboard action: " + std::to_string(action),
                              LOG_TYPE_ERROR );
}


void AbsCameraControls::onKeyboardPress( KeyEnum key,
                                         KeyEnum modifiers )
{
    if ( key == GLFW_KEY_ESCAPE )
        this->_p_window->endLoop();
}


void AbsCameraControls::bindToWindow( WindowPtr p_window )
{
    p_window->log( "Bind camera controls" );
    p_window->setMouseButtonCallback( std::bind( &AbsCameraControls::mouseButtonCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
    p_window->setCursorPositionCallback( std::bind( &AbsCameraControls::cursorPositionCallback, this, std::placeholders::_1, std::placeholders::_2 ) );
    p_window->setCursorEnterCallback( std::bind( &AbsCameraControls::cursorEnterCallback, this, std::placeholders::_1 ) );
    p_window->setScrollCallback( std::bind( &AbsCameraControls::scrollCallback, this, std::placeholders::_1, std::placeholders::_2 ) );
    p_window->setKeyboardCallback( std::bind( &AbsCameraControls::keyboardCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
    this->_p_window = p_window;
}


} // namespace cie::gl