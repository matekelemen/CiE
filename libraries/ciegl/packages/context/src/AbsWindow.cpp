// --- External Includes ---
#include "glad/glad.h"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsWindow.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::gl {


/* --- DEFAULT CALLBACKS --- */
namespace detail {

void defaultMouseButtonCallback( KeyEnum button,
                                 KeyEnum action,
                                 KeyEnum modifiers )
{
}

void defaultCursorPositionCallback( double x,
                                    double y )
{
}

void defaultCursorEnterCallback( KeyEnum entered )
{
}

void defaultScrollCallback( double xOffset,
                            double yOffset )
{
}

void defaultKeyboardCallback( KeyEnum key,
                              KeyEnum action,
                              KeyEnum modifiers )
{
}

void defaultWindowResizeCallback( Size width,
                                  Size height )
{
    glViewport( 0, 0, width, height );
}

} // namespace detail


AbsWindow::AbsWindow( Size id,
                      const std::string& r_name,
                      Size width,
                      Size height,
                      utils::Logger& r_logger ) :
    utils::IDObject<Size>( id ),
    utils::Loggee( r_logger, r_name ),
    utils::observer::Subject(),
    _size( width, height ),
    _scenes(),
    _continueLooping(false),
    _mouseButtonCallback( detail::defaultMouseButtonCallback ),
    _cursorPositionCallback( detail::defaultCursorPositionCallback ),
    _cursorEnterCallback( detail::defaultCursorEnterCallback ),
    _scrollCallback( detail::defaultScrollCallback ),
    _keyboardCallback( detail::defaultKeyboardCallback ),
    _windowResizeCallback( detail::defaultWindowResizeCallback )
{
}


AbsWindow::~AbsWindow()
{
    auto localBlock = this->newBlock( "close window" );
    this->logID( "", this->getID() );
}


void AbsWindow::update()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->activate();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    this->notifyObservers();

    for ( auto& rp_scene : _scenes )
        rp_scene->update();

    this->update_impl();

    CIE_END_EXCEPTION_TRACING
}


void AbsWindow::setSize( Size width,
                         Size height )
{
    CIE_BEGIN_EXCEPTION_TRACING

    setSize_impl( width, height );
    _size = std::make_pair( width, height );

    CIE_END_EXCEPTION_TRACING
}


const std::pair<Size,Size>& AbsWindow::getSize() const
{
    return _size;
}


void AbsWindow::beginLoop()
{
    if ( this->_continueLooping )
        this->log( "Attempt to begin loop while looping",
                   LOG_TYPE_WARNING );

    this->_continueLooping = true;

    auto localBlock = this->newBlock( "Event loop" );

    while( this->_continueLooping )
    { this->update(); }
}


void AbsWindow::endLoop()
{
    this->_continueLooping = false;
}


void AbsWindow::addScene( ScenePtr p_scene )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( p_scene )

    auto it_scene = std::find(
        _scenes.begin(),
        _scenes.end(),
        p_scene
    );

    if ( it_scene != _scenes.end() )
        CIE_THROW( Exception, "Attempt to add existing Scene to Window" )

    _scenes.push_back( p_scene );

    CIE_END_EXCEPTION_TRACING
}


void AbsWindow::removeScene( ScenePtr p_scene )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it_scene = std::find(
        _scenes.begin(),
        _scenes.end(),
        p_scene
    );

    if ( it_scene == _scenes.end() )
        CIE_THROW( Exception, "Attempt to remove non-existtent Scene from Window" )

    _scenes.erase( it_scene );

    CIE_END_EXCEPTION_TRACING
}


const typename AbsWindow::scene_container&
AbsWindow::scenes() const
{
    return _scenes;
}


} // namespace cie::gl