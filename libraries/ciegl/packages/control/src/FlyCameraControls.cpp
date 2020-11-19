// --- External Includes ---
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/control/inc/FlyCameraControls.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <fstream>
#include <algorithm>
#include <functional>
#include <cmath>


namespace cie::gl {


FlyCameraControls::FlyCameraControls() :
    MappedCameraControls(),
    _movementScale( 1.0 ),
    _rotationScale( 0.5 * M_PI / 180.0 ),
    _zoomScale( 1.1 ),
    _x( 0.0 ),
    _y( 0.0 ),
    _dx( 0.0 ),
    _dy( 0.0 ),
    _isLeftMouseButtonPressed( false ),
    _isRightMouseButtonPressed( false ),
    _verbose( false )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Set default controls
    std::string confiFilePath = SOURCE_PATH / "libraries/ciegl/data/configuration/controls/FlyCameraControls.xml";
    std::ifstream configFile( confiFilePath );

    if ( !configFile.is_open() )
        CIE_THROW( Exception, "Failed to open control config file: " + confiFilePath );

    this->configure( configFile );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::setMovementScale( double scale )
{
    if ( scale < 0.0 )
        this->_p_camera->log( "Movement scale must be non-negative (" + std::to_string(scale) + ")",
                              LOG_TYPE_ERROR );
    
    this->_movementScale = scale;
}


void FlyCameraControls::setRotationScale( double scale )
{
    if ( scale < 0.0 )
        this->_p_camera->log( "Rotation scale must be non-negative (" + std::to_string(scale) + ")",
                              LOG_TYPE_ERROR );
    
    this->_rotationScale = scale;
}


void FlyCameraControls::setZoomScale( double scale )
{
    if ( scale <= 0.0 )
        this->_p_camera->log( "Zoom scale must be positive (" + std::to_string(scale) + ")",
                              LOG_TYPE_ERROR );
    this->_zoomScale = scale;
}


void FlyCameraControls::moveForward()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_camera->translate(
        this->_movementScale * this->_p_camera->direction()
    );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::moveBackward()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_camera->translate(
        -this->_movementScale * this->_p_camera->direction()
    );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::strafeLeft()
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto unitVectorRight = glm::cross(
        this->_p_camera->direction(),
        this->_p_camera->up()
    );

    this->_p_camera->translate(
        -this->_movementScale * unitVectorRight
    );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::strafeRight()
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto unitVectorRight = glm::cross(
        this->_p_camera->direction(),
        this->_p_camera->up()
    );

    this->_p_camera->translate(
        this->_movementScale * unitVectorRight
    );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::moveUp()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_camera->translate(
        this->_movementScale * this->_p_camera->up()
    );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::moveDown()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_camera->translate(
        -this->_movementScale * this->_p_camera->up()
    );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::rotate()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( this->_dx!=0.0 || this->_dy!=0.0 )
    {
        auto unitVectorRight = glm::cross(
            this->_p_camera->direction(),
            this->_p_camera->up()
        );

        auto axis = this->_dy * unitVectorRight
                    - this->_dx * this->_p_camera->up();

        this->_p_camera->rotate( this->_rotationScale,
                                 axis,
                                 this->_p_camera->position() );
    }

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::rollCounterClockwise()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_camera->rotateRoll( -this->_rotationScale );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::rollClockwise()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_camera->rotateRoll( this->_rotationScale );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::zoomIn()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_camera->zoom( this->_zoomScale );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::zoomOut()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_camera->zoom( 1.0 / this->_zoomScale );

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::onMouseButtonPress( KeyEnum button,
                                            KeyEnum modifiers )
{
    if ( button == GLFW_MOUSE_BUTTON_1 )
        this->_isLeftMouseButtonPressed = true;
    else if ( button == GLFW_MOUSE_BUTTON_2 )
        this->_isRightMouseButtonPressed = true;
}


void FlyCameraControls::onMouseButtonRelease( KeyEnum button,
                                              KeyEnum modifiers )
{
    if ( button == GLFW_MOUSE_BUTTON_1 )
        this->_isLeftMouseButtonPressed = false;
    else if ( button == GLFW_MOUSE_BUTTON_2 )
        this->_isRightMouseButtonPressed = false;
}


void FlyCameraControls::onCursorMovement( double x,
                                          double y )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Flip y
    y = this->_p_window->getSize().second - y;

    // Update
    this->_dx = x - this->_x;
    this->_dy = y - this->_y;
    this->_x  = x;
    this->_y  = y;

    // Rotate
    if ( this->_isLeftMouseButtonPressed || this->_isRightMouseButtonPressed )
        this->rotate();

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::onHorizontalScroll( double offset )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( offset > 0.0 )
        this->strafeRight();
    else if ( offset < 0.0 )
        this->strafeLeft();

    CIE_END_EXCEPTION_TRACING
}


void FlyCameraControls::onVerticalScroll( double offset )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( offset > 0.0 )
        this->zoomIn();
    else if ( offset < 0.0 )
        this->zoomOut();

    CIE_END_EXCEPTION_TRACING
}


FlyCameraControls::configuration_map_type
FlyCameraControls::makeConfigurationMap( FlyCameraControls::configuration_contents& r_configContents )
{
    CIE_BEGIN_EXCEPTION_TRACING

    FlyCameraControls::configuration_map_type configMap;
    
    auto findControl = [&r_configContents, this]( const std::string& r_controlName )
    -> typename FlyCameraControls::configuration_contents::iterator
    {
        auto it = std::find_if(
            r_configContents.begin(),
            r_configContents.end(),
            [&r_controlName]( typename FlyCameraControls::configuration_contents::value_type& r_pair )
            {
                if ( r_controlName == r_pair.first )
                    return true;
                return false;
            }
        );

        if ( it == r_configContents.end() )
            this->_p_camera->log( "Control '" + r_controlName + "' not found in the configuration file",
                                  LOG_TYPE_ERROR );

        return it;
    };

    auto it_pair = findControl( "moveForward" );
    configMap.emplace(
        it_pair->second,
        std::bind( &FlyCameraControls::moveForward, this )
    );

    it_pair = findControl( "moveBackward" );
    configMap.emplace(
        it_pair->second,
        std::bind( &FlyCameraControls::moveBackward, this )
    );

    it_pair = findControl( "strafeLeft" );
    configMap.emplace(
        it_pair->second,
        std::bind( &FlyCameraControls::strafeLeft, this )
    );

    it_pair = findControl( "strafeRight" );
    configMap.emplace(
        it_pair->second,
        std::bind( &FlyCameraControls::strafeRight, this )
    );

    it_pair = findControl( "moveUp" );
    configMap.emplace(
        it_pair->second,
        std::bind( &FlyCameraControls::moveUp, this )
    );

    it_pair = findControl( "moveDown" );
    configMap.emplace(
        it_pair->second,
        std::bind( &FlyCameraControls::moveDown, this )
    );

    it_pair = findControl( "rollCounterClockwise" );
    configMap.emplace(
        it_pair->second,
        std::bind( &FlyCameraControls::rollCounterClockwise, this )
    );

    it_pair = findControl( "rollClockwise" );
    configMap.emplace(
        it_pair->second,
        std::bind( &FlyCameraControls::rollClockwise, this )
    );

    // Escape for closing the window
    configMap.emplace(
        GLFW_KEY_ESCAPE,
        std::bind( &AbsWindow::endLoop, this->_p_window.get() )
    );

    return configMap;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl