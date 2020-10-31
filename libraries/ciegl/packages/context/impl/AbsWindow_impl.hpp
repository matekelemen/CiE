#ifndef CIE_GL_ABS_WINDOW_IMPL_HPP
#define CIE_GL_ABS_WINDOW_IMPL_HPP

namespace cie::gl {


template < class SceneType,
           class ...Args >
inline ScenePtr
AbsWindow::makeScene( const std::string& r_name, Args&&... args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_scene = ScenePtr(
        new SceneType( this->logger(),
                       r_name,
                       std::forward<Args>(args)... )
    );

    this->addScene( p_scene );

    return p_scene;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl

#endif