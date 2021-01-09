// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shapes/inc/AbsGLShape.hpp"


namespace cie::gl {



AbsGLShape::AbsGLShape( AbsGLShape::attribute_container_ptr p_attributes ) :
    _p_attributes( p_attributes )
{
}                             


} // namespace cie::gl