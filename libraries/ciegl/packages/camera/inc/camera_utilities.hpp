#ifndef CIE_GL_CAMERA_UTILITIES_HPP
#define CIE_GL_CAMERA_UTILITIES_HPP

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Box.hpp"

// --- Internal Includes ---
#include "ciegl/packages/camera/inc/Camera.hpp"
#include "ciegl/packages/camera/inc/OrthographicProjection.hpp"
#include "ciegl/packages/camera/inc/PerspectiveProjection.hpp"


namespace cie::gl {


template < class ProjectionType,
           concepts::NumericType CoordinateType >
void fitViewToBox( AbsCamera& r_camera,
                   const csg::Box<3,CoordinateType>& r_box );


} // namespace cie::gl

#include "ciegl/packages/camera/impl/camera_utilities_impl.hpp"

#endif