#ifndef GLVISUALIZATION_CALLBACK_GROUP_IMPL_HPP
#define GLVISUALIZATION_CALLBACK_GROUP_IMPL_HPP

namespace cie {
namespace gl {
    

template <class CallbackType>
CallbackType makeCallback(  CallbackType callback,
                            DrawManager* drawManager )
{
    if ( glfwGetWindowUserPointer( drawManager->context()->window()) == nullptr )
        glfwSetWindowUserPointer( drawManager->context()->window(), static_cast<void*>(drawManager) );
    return callback;
}


}
}

#endif