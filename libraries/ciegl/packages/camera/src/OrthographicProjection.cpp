// --- External Includes ---
#include <glm/gtc/matrix_transform.hpp>

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/camera/inc/OrthographicProjection.hpp"

// --- STL Includes ---
#include <cmath>


namespace cie::gl {


void OrthographicProjection::zoom_impl( double scale )
{
    CIE_RUNTIME_GEOMETRY_CHECK(
        scale > 0,
        "Invalid zoom scale: " + std::to_string(scale)
    )

    this->_fieldOfView *= scale;
}


void OrthographicProjection::updateProjectionMatrix_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto tmpSizes   = this->sizes();
    tmpSizes.first  /= 2.0;
    tmpSizes.second /= 2.0;

    this->_projectionMatrix = glm::ortho( -tmpSizes.first,
                                          tmpSizes.first,
                                          -tmpSizes.second,
                                          tmpSizes.second,
                                          this->_clippingPlanes.first,
                                          this->_clippingPlanes.second );

    CIE_END_EXCEPTION_TRACING
}


std::pair<double,double> OrthographicProjection::sizes() const
{
    std::pair<double,double> sizes;

    sizes.first  = 2.0 * this->_clippingPlanes.first
                   * std::tan( this->_fieldOfView * M_PI / 360.0 );
    sizes.second = this->_aspectRatio * sizes.first;

    return sizes;
}


} // namespace cie::gl