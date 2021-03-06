// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shapes/inc/AbsGLShape.hpp"


namespace cie::gl {



AbsGLShape::AbsGLShape( AbsGLShape::attribute_container_ptr p_attributes ) :
    _p_attributes( p_attributes )
{
}


const AbsGLShape::attribute_container_ptr AbsGLShape::attributes() const
{
    return this->_p_attributes;
}


AbsGLShape::attribute_container_ptr AbsGLShape::attributes()
{
    return this->_p_attributes;
}


} // namespace cie::gl