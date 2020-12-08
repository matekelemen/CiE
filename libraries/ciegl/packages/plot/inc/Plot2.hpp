#ifndef CIE_GL_PLOT_PLOT_2_HPP
#define CIE_GL_PLOT_PLOT_2_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "ciegl/packages/plot/inc/AbsPlot.hpp"
#include "ciegl/packages/buffer/inc/Vertex2.hpp"
#include "ciegl/packages/scene/inc/Scene.hpp"

#include "ciegl/packages/camera/inc/Camera.hpp"
#include "ciegl/packages/camera/inc/OrthographicProjection.hpp"
#include "ciegl/packages/control/inc/PanZoomCameraControls.hpp"

// --- STL Includes ---
#include <vector>
#include <memory>


namespace cie::gl {


class Plot2;
using Plot2Ptr = std::shared_ptr<Plot2>;


class Plot2 : public AbsPlot
{
public:
    using vertex_type      = Vertex2;
    using vertex_ptr       = Vertex2Ptr;
    using vertex_container = std::vector<vertex_ptr>;

    using camera_type      = Camera<OrthographicProjection>;
    using controls_type    = PanZoomCameraControls;
    using controls_ptr     = std::shared_ptr<controls_type>;

public:
    Plot2( WindowPtr p_window );
    Plot2();

    template < concepts::NumericContainer XContainer,
               concepts::NumericContainer YContainer >
    friend void plot( const XContainer& r_xContainer,
                      const YContainer& r_yContainer );

    void fit() override;

private:
    void initializeScene();

protected:
    /**
     * @brief internal Scene class used by Plot2
     */
    class Plot2Scene : public Scene
    {
    public:
        Plot2Scene( utils::Logger& r_logger,
                    const std::string& r_name,
                    AttributeContainerPtr p_attributes );
        CameraPtr getCamera();
        
        /**
         * Sets a flag that forces all attributes to be rebuffered
         * on the next update
         */
        void setUpdateFlag();

    private:
        void update_impl() override;

    private:
        AttributeContainerPtr _p_attributes;
        bool                  _updateFlag;
    };

protected:
    vertex_container            _vertices;
    CameraPtr                   _p_camera;
    std::shared_ptr<Plot2Scene> _p_scene;
    controls_ptr                _p_controls;
};


} // namespace cie::gl

#include "ciegl/packages/plot/impl/Plot2_impl.hpp"

#endif