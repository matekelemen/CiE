// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWWindow.hpp"


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

    // Set window pointer
    glfwSetWindowUserPointer( _p_window, this );

    // Set window resize calbback
    auto resizeCallback = []( GLFWwindow* p_window, int width, int height )
    {
        auto p_GLFWWindow = static_cast<GLFWWindow*>( glfwGetWindowUserPointer( p_window ) );
        p_GLFWWindow->onResize( p_window, width, height );
    };
    glfwSetWindowSizeCallback( _p_window, resizeCallback );

    // Set key callback
    auto keyCallback = []( GLFWwindow* p_window, int key, int scanCode, int action, int modifier )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        auto p_this = static_cast<GLFWWindow*>( glfwGetWindowUserPointer(p_window) );
        CIE_CHECK_POINTER( p_this )
        p_this->_keyCallback( key, action, modifier );
        CIE_END_EXCEPTION_TRACING
    };
    glfwSetKeyCallback( _p_window, keyCallback );

    // Set mouse callbacks
    auto cursorCallback = []( GLFWwindow* p_window, double x, double y )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        auto p_this = static_cast<GLFWWindow*>( glfwGetWindowUserPointer(p_window) );
        CIE_CHECK_POINTER( p_this )
        p_this->_mouseCallback( x, y, 0, 0, 0 );
        CIE_END_EXCEPTION_TRACING
    };
    glfwSetCursorPosCallback( _p_window, cursorCallback );

    auto mouseClickCallback = []( GLFWwindow* p_window, int button, int action, int modifier )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        double x,y;
        glfwGetCursorPos( p_window, &x, &y );
        auto p_this = static_cast<GLFWWindow*>( glfwGetWindowUserPointer(p_window) );
        CIE_CHECK_POINTER( p_this )
        p_this->_mouseCallback( x, y, button, action, modifier );
        CIE_END_EXCEPTION_TRACING
    };
    glfwSetMouseButtonCallback( _p_window, mouseClickCallback );

    // Check whether the new window has the requested parameters
    int checkWidth, checkHeight;
    glfwGetFramebufferSize( _p_window, &checkWidth, &checkHeight );
    if ( Size(checkWidth)!=width || Size(checkHeight)!=height )
        CIE_THROW( Exception, "Created window is not of the requested size!" );

    CIE_END_EXCEPTION_TRACING
}


GLFWWindow::~GLFWWindow()
{
    glfwSetWindowShouldClose( _p_window, 1 );
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

    // Check for polling errors
    GLint err = glGetError();
    if (err!=0)
        this->log( "Error before updating! Error code: " + std::to_string(err),
                   LOG_TYPE_ERROR );

    glfwPollEvents();

    // Check for polling errors
    err = glGetError();
    if (err!=0)
        this->log( "Error polling events! Error code: " + std::to_string(err),
                   LOG_TYPE_ERROR );
    
    glfwSwapBuffers( this->_p_window );

    // Check for buffer errors
    err = glGetError();
    if (err!=0)
        this->log( "Error swapping buffers! Error code: " + std::to_string(err),
                   LOG_TYPE_ERROR );

    CIE_END_EXCEPTION_TRACING
}


void GLFWWindow::onResize( GLFWwindow* p_window,
                     int width,
                     int height )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_size = std::make_pair<Size,Size>( width, height );

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


std::pair<double,double> GLFWWindow::getCursorPosition()
{
    CIE_BEGIN_EXCEPTION_TRACING

    double x, y;
    glfwGetCursorPos( _p_window, &x, &y );
    return std::make_pair( x, y );

    CIE_END_EXCEPTION_TRACING
}


namespace detail {
GLFWwindow* getGLFWwindow( WindowPtr p_window )
{
    return dynamic_pointer_cast<GLFWWindow>( p_window )->get();
}
} // namespace detail


} // namespace cie::gl