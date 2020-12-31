// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/plot/inc/AbsPlot.hpp"
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "cmake_variables.hpp"


namespace cie::gl {


const std::filesystem::path PLOT_LOG_PATH = OUTPUT_PATH / "plot.log";


AbsPlot::AbsPlot( WindowPtr p_window ) :
    _p_context( GLFWContextSingleton::get( PLOT_LOG_PATH, true ) ),
    _p_window( p_window ),
    _p_attributes( new AttributeContainer )
{
}


AbsPlot::AbsPlot() :
    _p_context( GLFWContextSingleton::get( PLOT_LOG_PATH, true ) ),
    _p_window( nullptr ),
    _p_attributes( new AttributeContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_window = this->_p_context->newWindow(
        800,
        600
    );

    CIE_END_EXCEPTION_TRACING
}


void AbsPlot::update()
{
    this->_p_window->update();
}


const AttributeContainer& AbsPlot::attributes() const
{
    return *this->_p_attributes;
}


} // namespace cie::gl