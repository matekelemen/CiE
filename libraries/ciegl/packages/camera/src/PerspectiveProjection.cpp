// --- External Includes ---
#include <glm/gtc/matrix_transform.hpp>

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/camera/inc/PerspectiveProjection.hpp"


namespace cie::gl {


void PerspectiveProjection::zoom_impl( double scale )
{
    CIE_RUNTIME_GEOMETRY_CHECK(
        scale > 0,
        "Invalid zoom scale: " + std::to_string(scale)
    )

    this->_fieldOfView *= 1.0 / scale;
}


void PerspectiveProjection::updateProjectionMatrix_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_projectionMatrix = glm::perspective( this->_fieldOfView,
                                                this->_aspectRatio,
                                                this->_clippingPlanes.first,
                                                this->_clippingPlanes.second );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl