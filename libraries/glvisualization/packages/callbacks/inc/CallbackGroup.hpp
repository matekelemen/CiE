#ifndef GLVISUALIZATION_CALLBACK_GROUP_HPP
#define GLVISUALIZATION_CALLBACK_GROUP_HPP

// --- Internal Includes ---
#include "../../context/inc/GLContext.hpp"
#include "../../drawing/inc/DrawManager.hpp"


namespace cie {
namespace gl {



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
};


}
}

#include "CallbackGroup_impl.hpp"

#endif