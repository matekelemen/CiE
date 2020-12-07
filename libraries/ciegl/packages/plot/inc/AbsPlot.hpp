#ifndef CIE_GL_PLOT_ABS_PLOT_HPP
#define CIE_GL_PLOT_ABS_PLOT_HPP

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsContext.hpp"
#include "ciegl/packages/context/inc/AbsWindow.hpp"
#include "ciegl/packages/buffer/inc/AttributeContainer.hpp"

// --- STL Includes ---
#include <vector>
#include <deque>
#include <memory>


namespace cie::gl {


class AbsPlot
{
public:
    AbsPlot( WindowPtr p_window );
    AbsPlot();

    virtual void update();

    const AttributeContainer& attributes() const;

protected:
    ContextPtr            _p_context;
    WindowPtr             _p_window;
    AttributeContainerPtr _p_attributes;
};


using PlotPtr = std::shared_ptr<AbsPlot>;


} // namespace cie::gl


#endif