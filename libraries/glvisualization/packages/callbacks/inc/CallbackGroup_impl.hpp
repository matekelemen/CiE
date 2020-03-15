#ifndef GLVISUALIZATION_CALLBACK_GROUP_IMPL_HPP
#define GLVISUALIZATION_CALLBACK_GROUP_IMPL_HPP

namespace cie {
namespace gl {
    

template <class CallbackType>
CallbackType makeCallback(  CallbackType callback,
                            GLContext* context,
                            DrawManager* drawManager )
{
    if ( glfwGetWindowUserPointer(context->window()) == nullptr )
        glfwSetWindowUserPointer( context->window(), drawManager );
    return callback;
}


}
}

#endif