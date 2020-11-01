// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsWindow.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::gl {


/* --- DEFAULT CALLBACKS --- */
namespace detail {
void defaultKeyCallback( Size key,
                         Size action,
                         Size modifier )
{
    std::cout << key << std::endl;
}

void defaultMouseCallback( double x,
                           double y,
                           Size button,
                           Size action,
                           Size modifier )
{
    std::cout << x << " " << y << std::endl;
}
} // namespace detail


AbsWindow::AbsWindow( Size id,
                      const std::string& r_name,
                      Size width,
                      Size height,
                      utils::Logger& r_logger ) :
    utils::IDObject<Size>( id ),
    utils::AbsSubject(),
    utils::Loggee( r_logger, r_name ),
    _size( width, height ),
    _scenes(),
    _keyCallback( detail::defaultKeyCallback ),
    _mouseCallback( detail::defaultMouseCallback )
{
}


void AbsWindow::update()
{
    CIE_BEGIN_EXCEPTION_TRACING

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


void AbsWindow::addScene( ScenePtr p_scene )
{
    CIE_BEGIN_EXCEPTION_TRACING

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


void AbsWindow::setKeyCallback( typename AbsWindow::key_callback_function keyCallback )
{
    _keyCallback = keyCallback;
}


void AbsWindow::setMouseCallback( typename AbsWindow::mouse_callback_function mouseCallback )
{
    _mouseCallback = mouseCallback;
}


} // namespace cie::gl