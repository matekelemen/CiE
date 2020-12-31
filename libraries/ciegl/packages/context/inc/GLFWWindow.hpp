#ifndef CIE_GL_GLFW_WINDOW_HPP
#define CIE_GL_GLFW_WINDOW_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsWindow.hpp"


namespace cie::gl {


class GLFWWindow final : public AbsWindow
{
public:
    friend class GLFWContext;

public:
    ~GLFWWindow();

    const GLFWwindow* get() const;
    GLFWwindow* get();

protected:
    virtual void setSize_impl( Size width,
                               Size height ) override;

    virtual void update_impl() override;

private:
    GLFWWindow( Size id,
                const std::string& r_name,
                Size width,
                Size height,
                utils::Logger& r_logger );

private:
    GLFWwindow* _p_window;
};


namespace detail {
/** Helper function for getting a GLFW window
 * from a WindowPtr interface
*/
GLFWwindow* getGLFWwindow( WindowPtr p_window );
} // namespace detail




} // namespace cie::gl


#endif