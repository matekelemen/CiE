// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/plot/inc/AbsPlot.hpp"
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "cmake_variables.hpp"


namespace cie::gl {


AbsPlot::AbsPlot( WindowPtr p_window ) :
    _p_window( p_window ),
    _p_attributes( new AttributeContainer )
{
    CIE_CHECK_POINTER( p_window )
}


AbsPlot::AbsPlot() :
    _p_window( nullptr ),
    _p_attributes( new AttributeContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_window = GLFWContextSingleton::get()->newWindow();

    CIE_END_EXCEPTION_TRACING
}


void AbsPlot::update()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_window->update();

    CIE_END_EXCEPTION_TRACING
}


void AbsPlot::show()
{
    CIE_BEGIN_EXCEPTION_TRACING
    
    this->_p_window->beginLoop();

    CIE_END_EXCEPTION_TRACING
}


const AttributeContainer& AbsPlot::attributes() const
{
    return *this->_p_attributes;
}


} // namespace cie::gl