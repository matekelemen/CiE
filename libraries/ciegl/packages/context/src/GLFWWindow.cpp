// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/utility/inc/GLError.hpp"


namespace cie::gl {


GLFWWindow::GLFWWindow( Size id,
                        const std::string& r_name,
                        Size width,
                        Size height,
                        utils::Logger& r_logger ) :
    AbsWindow( id,
               r_name,
               width,
               height,
               r_logger ),
    _p_window( nullptr )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Construct window
    _p_window   = glfwCreateWindow( width, 
                                    height,
                                    r_name.c_str(),
                                    nullptr,
                                    nullptr );

    if ( !_p_window )
        CIE_THROW( Exception, "Failed to create window" )

    glfwMakeContextCurrent( _p_window );
    this->log( "Make context current" );

    // Set window pointer
    glfwSetWindowUserPointer( _p_window, this );

    // Set callbacks
    auto mouseButtonCallback = []( GLFWwindow* p_window, int button, int action, int modifiers )
    {
        auto p_this = static_cast<GLFWWindow*>( glfwGetWindowUserPointer(p_window) );
        CIE_CHECK_POINTER( p_this )
        p_this->_mouseButtonCallback( button, action, modifiers );
    };
    glfwSetMouseButtonCallback( _p_window, mouseButtonCallback );

    auto cursorPositionCallback = []( GLFWwindow* p_window, double x, double y )
    {
        auto p_this = static_cast<GLFWWindow*>( glfwGetWindowUserPointer(p_window) );
        CIE_CHECK_POINTER( p_this )
        p_this->_cursorPositionCallback( x, y );
    };
    glfwSetCursorPosCallback( _p_window, cursorPositionCallback );

    auto cursorEnterCallback = []( GLFWwindow* p_window, int entered )
    {
        auto p_this = static_cast<GLFWWindow*>( glfwGetWindowUserPointer(p_window) );
        CIE_CHECK_POINTER( p_this )
        p_this->_cursorEnterCallback( entered );
    };
    glfwSetCursorEnterCallback( _p_window, cursorEnterCallback );

    auto scrollCallback = []( GLFWwindow* p_window, double xOffset, double yOffset )
    {
        auto p_this = static_cast<GLFWWindow*>( glfwGetWindowUserPointer(p_window) );
        CIE_CHECK_POINTER( p_this )
        p_this->_scrollCallback( xOffset, yOffset );
    };
    glfwSetScrollCallback( _p_window, scrollCallback );

    auto keyboardCallback = []( GLFWwindow* p_window, int key, int scanCode, int action, int modifiers )
    {
        auto p_this = static_cast<GLFWWindow*>( glfwGetWindowUserPointer(p_window) );
        CIE_CHECK_POINTER( p_this )
        p_this->_keyboardCallback( key, action, modifiers );
    };
    glfwSetKeyCallback( _p_window, keyboardCallback );

    auto windowResizeCallback = []( GLFWwindow* p_window, int width, int height )
    {
        auto p_this = static_cast<GLFWWindow*>( glfwGetWindowUserPointer(p_window) );
        CIE_CHECK_POINTER( p_this )

        p_this->_size.first  = Size(width);
        p_this->_size.second = Size(height);

        for ( auto& rp_scene : p_this->_scenes )
            for ( auto& rp_camera : rp_scene->cameras() )
                rp_camera->setAspectRatio( double(width)/height );

        p_this->_windowResizeCallback( Size(width), Size(height) );
    };
    glfwSetWindowSizeCallback( _p_window, windowResizeCallback );

    // Check whether the new window has the requested parameters
    int checkWidth, checkHeight;
    glfwGetFramebufferSize( _p_window, &checkWidth, &checkHeight );
    if ( Size(checkWidth)!=width || Size(checkHeight)!=height )
        CIE_THROW( Exception, "Created window is not of the requested size!" );

    CIE_END_EXCEPTION_TRACING
}


GLFWWindow::~GLFWWindow()
{
    glfwDestroyWindow( this->_p_window );
}


void GLFWWindow::activate()
{
    CIE_BEGIN_EXCEPTION_TRACING

    glfwMakeContextCurrent( _p_window );

    CIE_END_EXCEPTION_TRACING
}


void GLFWWindow::setSize_impl( Size width,
                               Size height )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( _p_window )
    glfwSetWindowSize( _p_window, width, height );

    CIE_END_EXCEPTION_TRACING
}


void GLFWWindow::update_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Check for external errors
    checkGLErrors( *this,
                   "Error before updating!" );

    glfwPollEvents();

    // Check for polling errors
    checkGLErrors( *this,
                   "Error polling events!" );
    
    glfwSwapBuffers( this->_p_window );

    // Check for buffer errors
    checkGLErrors( *this,
                   "Error swapping buffers!" );

    CIE_END_EXCEPTION_TRACING
}


const GLFWwindow* GLFWWindow::get() const
{
    return _p_window;
}


GLFWwindow* GLFWWindow::get()
{
    return _p_window;
}


namespace detail {
GLFWwindow* getGLFWwindow( WindowPtr p_window )
{
    return dynamic_pointer_cast<GLFWWindow>( p_window )->get();
}
} // namespace detail


} // namespace cie::gl