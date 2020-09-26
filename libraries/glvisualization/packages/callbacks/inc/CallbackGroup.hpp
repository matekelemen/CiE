#ifndef GLVISUALIZATION_CALLBACK_GROUP_HPP
#define GLVISUALIZATION_CALLBACK_GROUP_HPP

// --- Internal Includes ---
#include "glvisualization/packages/context/inc/GLContext.hpp"
#include "glvisualization/packages/drawing/inc/DrawManager.hpp"


namespace cie::gl {



template <class CallbackType>
CallbackType makeCallback(  CallbackType callback,
                            DrawManager* drawManager );



struct CallbackGroup
{
    static void keyCallback(    WindowPtr window,
                                int key,
                                int scanCode,
                                int action,
                                int mods    );

    static void mouseCallback(  WindowPtr window,
                                int button,
                                int action,
                                int mods    );

    static void cursorCallback( WindowPtr window,
                                double x,
                                double y   );

    template <class CameraType>
    static CameraType* getCameraPtr( WindowPtr window );
};


struct ArcballCallbacks : CallbackGroup
{
    static void mouseCallback(  WindowPtr window,
                                int button,
                                int action,
                                int mods    );

    static void cursorCallback( WindowPtr window,
                                double x,
                                double y   ); 
};


} // namespace cie::gl

#include "glvisualization/packages/callbacks/impl/CallbackGroup_impl.hpp"

#endif