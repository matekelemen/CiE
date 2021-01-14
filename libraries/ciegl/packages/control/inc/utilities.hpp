#ifndef CIE_GL_CONTROLS_UTILITIES_HPP
#define CIE_GL_CONTROLS_UTILITIES_HPP

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsWindow.hpp"


namespace cie::gl {


/**
 * Transform input screen coordinates ( top left corner as origin, downward vertical y axis )
 * into unit a cartesian system ( bottom left corner as origin, upward vertical y axis ).
 */
void transformScreenCoordinates( double& x, double& y, WindowWeakPtr wp_window );


} // namespace cie::gl


#endif