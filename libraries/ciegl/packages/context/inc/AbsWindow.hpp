#ifndef CIE_GL_ABS_WINDOW_HPP
#define CIE_GL_ABS_WINDOW_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"
#include "cieutils/packages/types/inc/NamedObject.hpp"
#include "cieutils/packages/types/inc/IDObject.hpp"
#include "cieutils/packages/logging/inc/Loggee.hpp"
#include "cieutils/packages/logging/inc/LoggerSingleton.hpp"
#include "cieutils/packages/observer/inc/Subject.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Scene.hpp"
#include "ciegl/packages/control/inc/callback_types.hpp"
#include "ciegl/packages/texture/inc/Image.hpp"

// --- STL Includes ---
#include <memory>
#include <utility>
#include <list>
#include <functional>
#include <filesystem>


namespace cie::gl {


class AbsWindow :
    public utils::IDObject<Size>,
    public utils::Loggee,
    public utils::observer::Subject
{
public:
    using scene_container           = std::list<ScenePtr>;
    using key_callback_function     = std::function<void(Size,Size,Size)>;              // <-- void( key, action, modifier )
    using mouse_callback_function   = std::function<void(double,double,Size,Size,Size)>;// <-- void( x, y, button, action, modifier )

public:
    AbsWindow(  Size id,
                const std::string& r_name,
                Size width,
                Size height,
                utils::Logger& r_logger = utils::LoggerSingleton::get() );

    ~AbsWindow();

    /// Update all scenes and draw
    virtual void update();

    /// Make window active
    virtual void activate() = 0;

    /// Resize this window. Calls setSize_impl internally
    void setSize( Size width,
                  Size height );

    const std::pair<Size,Size>& getSize() const;

    /// Construct a new scene and add it to this window
    template < class SceneType,
               class ...Args>
    std::shared_ptr<SceneType> makeScene( Args&&... args );

    /// Attempt to remove a scene from this window
    virtual void removeScene( ScenePtr p_scene );

    const scene_container& scenes() const;

    void beginLoop();
    void endLoop();

    /** Copy window data into an RGB image and write it to disk
     *  @param r_outputPath relative or full path of the new image file
     *  @param p_targetImage preallocated image. If none is given, allocation
     *  is performed internally
     */
    void screenshot( const std::filesystem::path& r_outputPath,
                     Image* p_targetImage = nullptr ) const;

    void setMouseButtonCallback( MouseButtonCallback function );
    void setCursorPositionCallback( CursorPositionCallback function );
    void setCursorEnterCallback( CursorEnterCallback function );
    void setScrollCallback( ScrollCallback function );
    void setKeyboardCallback( KeyboardCallback function );
    void setWindowResizeCallback( WindowResizeCallback function );

protected:
    /// Attempt to add a scene to this window
    virtual void addScene( ScenePtr p_scene );

    virtual void setSize_impl(  Size width,
                                Size height ) = 0;

    virtual void update_impl()
    {}

protected:
    std::pair<Size,Size>    _size;
    scene_container         _scenes;

    bool                    _continueLooping;

    MouseButtonCallback     _mouseButtonCallback;
    CursorPositionCallback  _cursorPositionCallback;
    CursorEnterCallback     _cursorEnterCallback;
    ScrollCallback          _scrollCallback;
    KeyboardCallback        _keyboardCallback;

    WindowResizeCallback    _windowResizeCallback;
};


using WindowPtr         = std::shared_ptr<AbsWindow>;
using WindowConstPtr    = std::shared_ptr<const AbsWindow>;
using WindowWeakPtr     = std::weak_ptr<AbsWindow>;


} // namespace cie::gl

#include "ciegl/packages/context/impl/AbsWindow_impl.hpp"

#endif