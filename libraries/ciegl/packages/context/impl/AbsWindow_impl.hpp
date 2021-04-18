#ifndef CIE_GL_ABS_WINDOW_IMPL_HPP
#define CIE_GL_ABS_WINDOW_IMPL_HPP

namespace cie::gl {


template < class SceneType,
           class ...Args >
inline std::shared_ptr<SceneType>
AbsWindow::makeScene( Args&&... args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto localBlock = this->newBlock( "construct new scene" );

    auto p_scene = std::make_shared<SceneType>( std::forward<Args>(args)... );

    this->log( p_scene->name() );

    this->addScene( p_scene );

    return p_scene;

    CIE_END_EXCEPTION_TRACING
}


inline void
AbsWindow::setMouseButtonCallback( MouseButtonCallback function )
{ this->_mouseButtonCallback = function; }


inline void
AbsWindow::setCursorPositionCallback( CursorPositionCallback function )
{ this->_cursorPositionCallback = function; }


inline void
AbsWindow::setCursorEnterCallback( CursorEnterCallback function )
{ this->_cursorEnterCallback = function; }


inline void
AbsWindow::setScrollCallback( ScrollCallback function )
{ this->_scrollCallback = function; }


inline void
AbsWindow::setKeyboardCallback( KeyboardCallback function )
{ this->_keyboardCallback = function; }


inline void
AbsWindow::setWindowResizeCallback( WindowResizeCallback function )
{ this->_windowResizeCallback = function; }


} // namespace cie::gl

#endif